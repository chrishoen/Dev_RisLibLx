/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "prnPrint.h"

#include "ris_priorities.h"
#include "risNetTcpMsgClientThread.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

TcpMsgClientThread::TcpMsgClientThread()
{
   mThreadPriority = get_default_tcp_client_thread_priority();

   mConnectionFlag=false;
   mFlags=0;
}

//******************************************************************************
// Configure:

void TcpMsgClientThread::configure(
   BaseMsgMonkeyCreator* aMonkeyCreator,
   char*                 aServerIpAddr,
   int                   aServerIpPort,
   SessionQCall*         aSessionQCall,
   RxMsgQCall*           aRxMsgQCall,
   int                   aFlags) 
{
   Prn::print(Prn::SocketInit1, "TcpClientThread::configure");

   mConnectionFlag=false;
   mFlags=aFlags;
   mSocketAddress.set(aServerIpAddr,aServerIpPort);
   mMonkeyCreator = aMonkeyCreator;

   if (aSessionQCall)
   {
      mSessionQCall = *aSessionQCall;
   }

   if (aRxMsgQCall)
   {
      mRxMsgQCall = *aRxMsgQCall;
   }
}

//******************************************************************************
// Thread init function, base class overload.
// It configures the socket.

void TcpMsgClientThread::threadInitFunction()
{
   Prn::print(Prn::SocketInit1, "TcpClientThread::threadInitFunction BEGIN");

   // Configure the socket
   mSocket.configure(mMonkeyCreator,mSocketAddress);

   Prn::print(Prn::SocketInit1, "TcpClientThread::threadInitFunction END");
}

//******************************************************************************
// Thread run function, base class overload.
// It contains a while loop that manages the connection to the server
// and receives messages.

void TcpMsgClientThread::threadRunFunction()
{
   Prn::print(Prn::SocketRun1, "TcpClientThread::threadRunFunction");
   
   //-----------------------------------------------------------
   // Loop

   mConnectionFlag=false;

   bool going=true;

   while(going)
   {
      //-----------------------------------------------------------
      // If no connection
      if (!mConnectionFlag)
      {
         // Try to connect
         if (mSocket.doConnect())
         {
            // Connection was established
            Prn::print(Prn::SocketRun1, "Connected");
            mConnectionFlag = true;

            // process a session change because a
            // new session has been established
            processSessionChange(true);
         }
         else 
         {
            // Connection was not established
            Prn::print(Prn::SocketRun3, "Not Connected");

            mConnectionFlag = false;

            // Close socket
            mSocket.doClose();
            mSocket.reconfigure();

            // Sleep
            threadSleep(500);
         }
      }
      //-----------------------------------------------------------
      // If connection
      else
      {
         // Try to receive a message with a blocking receive call
         // If a message was received then process it.
         // If a message was not received then the connection was lost.  
         ByteContent* tMsg=0;
         if (mSocket.doReceiveMsg(tMsg))
         {
            // Message was correctly received
            Prn::print(Prn::SocketRun2, "Recv message %d",mSocket.mRxMsgCount);

            // process the receive message
            if (tMsg)
            {
               processRxMsg(tMsg);
            }
         }
         else
         {
            // Message was not correctly received, so
            // Connection was lost
            Prn::print(Prn::SocketRun1, "Recv failed, Connection lost");
            mConnectionFlag = false;

            // process a session change because a
            // new session has been disestablished
            processSessionChange(false);
         }
      }
      //-----------------------------------------------------------
      // If termination request, exit the loop
      // This is set by shutdown, see below.
      if (mTerminateFlag)
      {
         going=false;
      }  
   }         
}

//******************************************************************************
// Thread exit function, base class overload.

void TcpMsgClientThread::threadExitFunction()
{
   Prn::print(Prn::SocketInit1, "TcpClientThread::threadExitFunction");
}

//******************************************************************************
// Shutdown, base class overload.
// This sets the terminate request flag and closes the socket.
//
// If the while loop in the threadRunFunction is blocked on doRecvMsg then
// closing the socket will cause doRecvMsg to return with false and 
// then the terminate request flag will be polled and the the
// threadRunFunction will exit.

void TcpMsgClientThread::shutdownThread()
{
   BaseThreadWithTermFlag::mTerminateFlag = true;

   mSocket.doClose();

   BaseThreadWithTermFlag::waitForThreadTerminate();
}
//******************************************************************************

void TcpMsgClientThread::sendMsg(ByteContent* aMsg)
{
   if (!aMsg) return;

   if (mConnectionFlag)
   {
      mSocket.doSendMsg(aMsg);
   }
   else
   {
      Prn::print(Prn::SocketRun1, "ERROR doSendMsg FAIL session invalid");
      delete aMsg;
   }

}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TcpMsgClientThread::processSessionChange(bool aEstablished)
{
   // Guard.
   if (!mSessionQCall.mExecuteCallPointer.isValid()) return;

   // Invoke the session qcall to notify that a session has
   // been established or disestablished
   // Create a new qcall, copied from the original, and invoke it.
   mSessionQCall(aEstablished);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TcpMsgClientThread::processRxMsg(Ris::ByteContent* aMsg)
{
   // Guard.
   if (!mRxMsgQCall.mExecuteCallPointer.isValid()) return;

   // Invoke the receive QCall
   // Create a new qcall, copied from the original, and invoke it.
   mRxMsgQCall(aMsg);
}

//******************************************************************************

}//namespace
}//namespace
