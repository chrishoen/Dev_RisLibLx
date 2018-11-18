/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "prnPrint.h"

#include "risThreadsPriorities.h"
#include "risNetTcpMsgServerThread.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

TcpMsgServerThread::TcpMsgServerThread(Settings& aSettings)
{
   // Base class variables.
   BaseClass::setThreadName("TcpMsgServer");
   BaseClass::setThreadPriority(aSettings.mThreadPriority);
   BaseClass::setThreadPrintLevel(aSettings.mPrintLevel);

   // Store settings.
   mSettings = aSettings;
   mSessionQCall = aSettings.mServerSessionQCall;
   mRxMsgQCall = aSettings.mServerRxMsgQCall;
   mMaxSessions = aSettings.mMaxSessions;

   // Member variables.
   mNumSessions=0;
   mListenFlag=false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function, base class overload.
// It configures the sockets.

void TcpMsgServerThread::threadInitFunction()
{
   Prn::print(Prn::SocketInitT1, "TcpServerThread::threadInitFunction");

   // Initialize and configure the hub socket.
   mHubSocket.initialize(mSettings);
   mHubSocket.configure();

   // Initialize the node sockets.
   for (int tSessionIndex=0;tSessionIndex<mMaxSessions;tSessionIndex++)
   {
      mNodeSocket[tSessionIndex].initialize(mSettings);
   }
}


//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function, base class overload.
// It contains a while loop that does a select call to manage the hub socket
// and the set of node sockets.

void TcpMsgServerThread::threadRunFunction()
{
   Prn::print(Prn::SocketInitT2, "TcpServerThread::threadRunFunction");
   
   // Do a nonblocking listen to put the hub socket in listen mode.
   mHubSocket.doListen();
   mListenFlag = true;
   Prn::print(Prn::SocketInitT2, "doListen %d %d",mHubSocket.mStatus,mHubSocket.mError);

   bool tGoing = true;
   while(tGoing)
   {
      //************************************************************************
      //************************************************************************
      //************************************************************************
      // Setup for the select call.

      // The read set is a set of sockets that is passed to the select
      // call. If a socket in the set gets a read condition then the
      // select call returns and the socket is still in the set. If 
      // a socket doesn't get a read condition then it's no longer in 
      // the set.

      mHubSocket.resetReadSet();

      // If listening then add the hub socket to the read set
      // It will signal if there is an accept call available.
      if (mListenFlag)
      {
         mHubSocket.addSelfToReadSet();
      }

      // For each valid session, add the node socket to the read set
      // The socket will signal if a recv is available.
      for (int tSessionIndex=0;tSessionIndex<mMaxSessions;tSessionIndex++)
      {
         if (mNodeSocket[tSessionIndex].mValidFlag)
         {
            mHubSocket.addToReadSet(&mNodeSocket[tSessionIndex]);
         }
      }

      //************************************************************************
      //************************************************************************
      //************************************************************************
      // Call select with the read set.
      // This blocks until the timeout or until one of the 
      // sockets in the read set has a read condition.

      int retVal = mHubSocket.selectOnReadSet();

      // Test for a termination request 
      if (mTerminateFlag)
      {
         tGoing=false;
      }   

      // Test if the select call was successful and that the
      // read set has at least one socket in it
      else if (retVal > 0)
      {
         //*********************************************************************
         //*********************************************************************
         //*********************************************************************
         // Test for accept available.
         // If the hub socket is still in the read set then an accept
         // call will not block, so a client is trying to establish
         // a connection with a connect call.
         
         if(mHubSocket.isSelfInReadSet())
         {
            // The hub socket is still in the read set. This means that a
            // client is trying to establish a connection with a connect 
            // call and that an accept call will be successful.

            // Allocate a new session index
            int tSessionIndex; 
            mSessionAllocator.pop(tSessionIndex); 

            // Call accept into the node socket at the new session index.
            mHubSocket.doAccept(mNodeSocket[tSessionIndex]);

            // Test the accept call.
            if (mHubSocket.mStatus>=0)
            {  
               // The accept call passed.
               // Set the new socket valid and update the session state.
               mNodeSocket[tSessionIndex].mValidFlag = true;
               mNumSessions++;

               // Process a session change because a new session
               // has been established.
               processSessionChange(tSessionIndex,true);

               Prn::print(Prn::SocketRxRunT1, "doAccept %d",tSessionIndex);

               // Test if the number of sessions has reached the maximum.
               if (mNumSessions == mMaxSessions)
               {
                  // The session limit was reached.
                  // Close the hub socket and set the listening state false.
                  // This will cause any client connect calls to be refused.
                  // The hub socket will be reopened when one of the sessions is closed.
                  Prn::print(Prn::SocketRxRunT1, "Session limit was reached, closing listener %d",mNumSessions);
                  mHubSocket.doClose();
                  mListenFlag=false;
               }
            }
            else
            {
               // The accept call failed. Dealloacte the session index.
               Prn::print(Prn::SocketErrorT1, "ERROR doAccept FAILED %d %d %d",mHubSocket.mStatus,mHubSocket.mError,tSessionIndex);
               mSessionAllocator.push(tSessionIndex); 
            }
         }
   
         //*********************************************************************
         //*********************************************************************
         //*********************************************************************
         // Test for recv available, for each valid session.
         // If the node socket is still in the read set then a recv
         // call will not block.

         for (int tSessionIndex = 0; tSessionIndex < mMaxSessions; tSessionIndex++)
         {
            if(mNodeSocket[tSessionIndex].mValidFlag)
            {
               // Test if the node socket is in the read set.
               if(mHubSocket.isInReadSet(&mNodeSocket[tSessionIndex]))
               {
                  // The node socket is in the read list. This means that 
                  // there is data available to be read from the socket.
 
                  // Attempt to receive a message on the node socket.
                  ByteContent* tMsg=0;
                  if (mNodeSocket[tSessionIndex].doReceiveMsg (tMsg))
                  {
                     // A valid message was received.
                     Prn::print(Prn::SocketRxRunT1, "Recv message %d %d",
                        tSessionIndex,
                        mNodeSocket[tSessionIndex].mRxCount);

                     // Process the received message.
                     if (tMsg)
                     {
                        processRxMsg(tSessionIndex,tMsg);
                     } 
                  }
                  else 
                  {
                     // The receive failed, so the connection was shutdown by the client.
                     // Therefore, disestablish the session.  
                     Prn::print(Prn::SocketRxRunT1, "Recv failed, closing session %d",tSessionIndex);
                     // Reset the socket.
                     mNodeSocket[tSessionIndex].doClose();
                     mNodeSocket[tSessionIndex].reset();
                     mNodeSocket[tSessionIndex].mValidFlag = false;
                     // Dealloacte the session index
                     mSessionAllocator.push(tSessionIndex);

                     // Process a session change because a session has been 
                     // disestablished.
                     processSessionChange(tSessionIndex,false);

                     // If not listening because the number of sessions had
                     // reached the limit then start listening again.
                     mNumSessions--;
                     if (!mListenFlag)
                     {
                        mHubSocket.reconfigure();
                        mHubSocket.doListen();
                        mListenFlag=true;
                        Prn::print(Prn::SocketRxRunT1, "opening listener, doListen %d %d",mHubSocket.mStatus,mHubSocket.mError);
                     } 
                  }   
               }
            }
         }
      }
      // Test if the select call failed.
      else if (retVal<0)
      {
         Prn::print(Prn::SocketErrorT1, "ERROR TcpServerThread::threadRunFunction select fail %d",retVal);
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function, base class overload.
// It closes all open sockets.

void TcpMsgServerThread::threadExitFunction()
{
   Prn::print(Prn::SocketInitT1, "TcpServerThread::threadExitFunction");

   mHubSocket.doClose();
   for (int tSessionIndex=0;tSessionIndex<mMaxSessions;tSessionIndex++)
   {
      if (mNodeSocket[tSessionIndex].mValidFlag)
      {
         mNodeSocket[tSessionIndex].doClose();
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Notify the parent thread that a session has changed. This is called by
// the threadRunFunction when a new session is established or an existing
// session is disestablished. It invokes the mSessionQCall that is
// registered at initialization.

void TcpMsgServerThread::processSessionChange(int aSessionIndex,bool aEstablished)
{
   // Invoke the session qcall to notify that a session has
   // been established or disestablished.
   mSessionQCall(aSessionIndex,aEstablished);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Pass a received message to the parent thread. This is called by the
// threadRunFunction when a message is received. It invokes the
// mRxMsgQCall that is registered at initialization.

void TcpMsgServerThread::processRxMsg(int aSessionIndex,Ris::ByteContent* aMsg)
{
   // Invoke the receive QCall.
   mRxMsgQCall(aSessionIndex,aMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a transmit message through the node socket at the session index
// to the client. It executes a blocking send() call in the context of
// the calling thread. It is protected by a mutex semaphore.

void TcpMsgServerThread::sendMsg(int aSessionIndex, ByteContent* aMsg)
{
   if (!aMsg) return;

   // Test if the session is valid
   if (mNodeSocket[aSessionIndex].mValidFlag)
   {
      // Send the message via the socket.
      mNodeSocket[aSessionIndex].doSendMsg(aMsg);
   }
   else
   {
      Prn::print(Prn::SocketErrorT2, "ERROR doSendMsg FAIL session invalid %d", aSessionIndex);
      delete aMsg;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace
