/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "prnPrint.h"

#include "ris_priorities.h"
#include "risNetTcpMsgServerThread.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// This initializes the socket. It stores the socket address to which the
// socket will be bound and calls socket and bind.

void TcpMsgServerHubSocket::configure(Sockets::SocketAddress aSocketAddress)
{
   reset();
   mLocal = aSocketAddress;
   
   doSocket();
   setOptionReuseAddr();
   doBind();

   if (mStatus==0)
   {
      Prn::print(Prn::SocketInit2, "TcpServerHubSocket $ %16s : %d",
         mLocal.mIpAddr.mString,
         mLocal.mPort);
   }
   else
   {
      Prn::print(Prn::SocketInit2, "TcpServerHubSocket $ %16s : %d $ %d %d",
         mLocal.mIpAddr.mString,
         mLocal.mPort,
         mStatus,
         mError);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This re-initializes the socket.

void TcpMsgServerHubSocket::reconfigure()
{
   doSocket();
   setOptionReuseAddr();
   doBind();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

TcpMsgServerThread::TcpMsgServerThread()
{
   mThreadPriority = get_default_tcp_server_thread_priority();

   mNumSessions=0;
   mMaxSessions=0;
   mListenFlag=false;
   mFlags=0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Configure:

void TcpMsgServerThread::configure(
   BaseMsgMonkeyCreator* aMonkeyCreator,
   char*                 aServerIpAddr,
   int                   aServerIpPort,
   int                   aMaxSessions, 
   SessionQCall*         aSessionQCall,
   RxMsgQCall*           aRxMsgQCall,
   int                   aFlags)
{
   Prn::print(Prn::SocketInit1, "TcpClientThread::configure");

   mSocketAddress.set(aServerIpAddr,aServerIpPort);
   mMaxSessions = aMaxSessions;
   mMonkeyCreator = aMonkeyCreator;
   mFlags = aFlags;

   mSessionQCall = *aSessionQCall;
   mRxMsgQCall   = *aRxMsgQCall;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function, base class overload.
// It configures the sockets.

void TcpMsgServerThread::threadInitFunction()
{
   Prn::print(Prn::SocketInit1, "TcpServerThread::threadInitFunction BEGIN");

   // Configure the hub socket
   mHubSocket.configure(mSocketAddress);

   // Register the message monkey for the node sockets
   for (int sessionIndex=0;sessionIndex<mMaxSessions;sessionIndex++)
   {
      mNodeSocket[sessionIndex].configure(mMonkeyCreator);
   }

   Prn::print(Prn::SocketInit1, "TcpServerThread::threadInitFunction END");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function, base class overload.
// It contains a while loop that does a select call to manage the hub socket
// and the set of node sockets.

void TcpMsgServerThread::threadRunFunction()
{
   Prn::print(Prn::SocketRun1, "TcpServerThread::threadRunFunction");
   
   //-------------------------------------------------------------------------
   // Loop

   // Do a nonblocking listen to put the hub socket in listen mode
   mHubSocket.doListen();
   mListenFlag=true;
   Prn::print(Prn::SocketRun1, "doListen %d %d",mHubSocket.mStatus,mHubSocket.mError);

   bool going=true;

   while(going)
   {
      //----------------------------------------------------------------------
      // setup for the select call

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
      for (int sessionIndex=0;sessionIndex<mMaxSessions;sessionIndex++)
      {
         if (mNodeSocket[sessionIndex].mValidFlag)
         {
            mHubSocket.addToReadSet(&mNodeSocket[sessionIndex]);
         }
      }

      //----------------------------------------------------------------------
      //----------------------------------------------------------------------
      //----------------------------------------------------------------------
      // Call select with the read set
      // This blocks until the timeout or until one of the 
      // sockets in the read set has a read condition

      int retVal = mHubSocket.selectOnReadSet();

      // Test for a termination request 
      if (mTerminateFlag)
      {
         going=false;
      }   

      // Test if the select call was successful and that the
      // read set has at least one socket in it
      else if (retVal > 0)
      {
         //--------------------------------------------------------------------
         // Test for accept available.
         // If the hub socket is still in the read set then an accept
         // call will not block, so a client is trying to establish
         // a connection with a connect call
         
         if(mHubSocket.isSelfInReadSet())
         {
               // The hub socket is still in the read set. This means that a
               // client is trying to establish a connection with a connect 
               // call and that an accept call will be successful.

               // Allocate a new session index
               int sessionIndex; 
               mSessionAllocator.pop(sessionIndex); 

               // Call accept into the node socket at the new session index
               mHubSocket.doAccept(mNodeSocket[sessionIndex]);

               // Test the accept call
               if (mHubSocket.mStatus>=0)
               {  
                  // The accept call passed.
                  // Set the new socket valid and update the session state.
                  mNodeSocket[sessionIndex].mValidFlag=true;
                  mNumSessions++;

                  // process a session change because a
                  // new session has been established
                  processSessionChange(sessionIndex,true);

                  Prn::print(Prn::SocketRun1, "doAccept %d",sessionIndex);

                  // Test if the number of sessions has reached the maximum
                  if (mNumSessions==mMaxSessions)
                  {
                     // The session limit was reached.
                     // Close the hub socket and set the listening state false.
                     // This will cause any client connect calls to be refused.
                     // The hub socket will be reopened when one of the sessions is closed.
                     Prn::print(Prn::SocketRun1, "Session limit was reached, closing listener %d",mNumSessions);
                     mHubSocket.doClose();
                     mListenFlag=false;
                  }
               }
               else
               {
                  // The accept call failed
                  Prn::print(Prn::SocketRun1, "ERROR doAccept FAILED %d %d %d",mHubSocket.mStatus,mHubSocket.mError,sessionIndex);
                  mSessionAllocator.push(sessionIndex); 
               }
         }
   
         //-------------------------------------------------------------------
         // Test for recv available, for each valid session.
         // If the node socket is still in the read set then a recv
         // call will not block.
         for (int sessionIndex=0;sessionIndex<mMaxSessions;sessionIndex++)
         {
            if(mNodeSocket[sessionIndex].mValidFlag)
            {
               // Test if the node socket is in the read set
               if(mHubSocket.isInReadSet(&mNodeSocket[sessionIndex]))
               {
                  // The node socket is in the read list. This means that 
                  // there is a data available to be read from the socket.
 
                  // Attempt to receive a message on the node socket.
                  ByteContent* tMsg=0;
                  if (mNodeSocket[sessionIndex].doReceiveMsg (tMsg))
                  {
                     // A valid message was received 
                     Prn::print(Prn::SocketRun2, "Recv message %d %d",
                        sessionIndex,
                        mNodeSocket[sessionIndex].mRxMsgCount);

                     // process the received message
                     if (tMsg)
                     {
                        processRxMsg(sessionIndex,tMsg);
                     } 
                  }
                  else 
                  {
                     // The receive failed, so the connection was shutdown by the client.
                     // Therefore, disestablish the session.  
                     Prn::print(Prn::SocketRun1, "Recv failed, closing session %d",sessionIndex);
                     // Reset the socket
                     mNodeSocket[sessionIndex].doClose();
                     mNodeSocket[sessionIndex].reset();
                     mNodeSocket[sessionIndex].mValidFlag=false;
                     // Dealloacte the session index
                     mSessionAllocator.push(sessionIndex);

                     // process a session change because a
                     // session has been disestablished
                     processSessionChange(sessionIndex,false);

                     // If not listening because the number of sessions had
                     // reached the limit then start listening again
                     mNumSessions--;
                     if (!mListenFlag)
                     {
                        mHubSocket.reconfigure();
                        mHubSocket.doListen();
                        mListenFlag=true;
                        Prn::print(Prn::SocketRun1, "opening listener, doListen %d %d",mHubSocket.mStatus,mHubSocket.mError);
                     } 
                  }   
               }
            }
         }
      }
      // Test if the select call failed
      else if (retVal<0)
      {
         Prn::print(Prn::SocketRun1, "ERROR TcpServerThread::threadRunFunction select fail %d",retVal);
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
   Prn::print(Prn::SocketInit1, "TcpServerThread::threadExitFunction");

   mHubSocket.doClose();
   for (int sessionIndex=0;sessionIndex<mMaxSessions;sessionIndex++)
   {
      if (mNodeSocket[sessionIndex].mValidFlag)
      {
         mNodeSocket[sessionIndex].doClose();
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TcpMsgServerThread::sendMsg(int aSessionIndex,ByteContent* aMsg)
{
   if (!aMsg) return;

   // Test if the session is valid
   if (mNodeSocket[aSessionIndex].mValidFlag)
   {
      // Send the message and update the state
      mNodeSocket[aSessionIndex].doSendMsg(aMsg);
      mNodeSocket[aSessionIndex].mTxMsgCount++;
   }
   else
   {
      Prn::print(Prn::SocketRun1, "ERROR doSendMsg FAIL session invalid %d",aSessionIndex);
      delete aMsg;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TcpMsgServerThread::processSessionChange(int aSessionIndex,bool aEstablished)
{
   // Invoke the session qcall to notify that a session has
   // been established or disestablished
   // Create a new qcall, copied from the original, and invoke it.
   mSessionQCall(aSessionIndex,aEstablished);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TcpMsgServerThread::processRxMsg(int aSessionIndex,Ris::ByteContent* aMsg)
{
   // Invoke the receive QCall
   // Create a new qcall, copied from the original, and invoke it.
   mRxMsgQCall(aSessionIndex,aMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace
