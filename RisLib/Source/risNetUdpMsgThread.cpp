/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "prnPrint.h"

#include "ris_priorities.h"
#include "risNetUdpMsgThread.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

UdpMsgThread::UdpMsgThread()
{
   mThreadPriority = get_default_udp_rx_thread_priority();
   mLocalIpAddress[0]=0;
   mLocalIpPort=0;
   mRemoteIpAddress[0]=0;
   mRemoteIpPort=0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Configure:

void UdpMsgThread::configure(
   BaseMsgMonkeyCreator* aMonkeyCreator,
   char*                 aLocalIpAddress,
   int                   aLocalIpPort,
   char*                 aRemoteIpAddress,
   int                   aRemoteIpPort,
   RxMsgQCall*           aMessageQCall)
{
   strcpy(mLocalIpAddress,aLocalIpAddress);
   mLocalIpPort = aLocalIpPort;

   strcpy(mRemoteIpAddress, aRemoteIpAddress);
   mRemoteIpPort = aRemoteIpPort;

   mMonkeyCreator = aMonkeyCreator;

   mRxMsgQCall = *aMessageQCall;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function, base class overload.
// It configures the socket.

void UdpMsgThread::threadInitFunction()
{
   Prn::print(Prn::SocketInit1, "UdpMsgThread::threadInitFunction BEGIN");

   mRxSocket.configure(
      mMonkeyCreator,
      mLocalIpAddress,
      mLocalIpPort);

   mTxSocket.configure(
      mMonkeyCreator,
      mRemoteIpAddress,
      mRemoteIpPort);

   Prn::print(Prn::SocketInit1, "UdpMsgThread::threadInitFunction END");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function, base class overload.
// It contains a while loop that manages the connection to the server
// and receives messages.

void  UdpMsgThread::threadRunFunction()
{
   Prn::print(Prn::SocketRun1, "UdpRxMsgThread::threadRunFunction");
   
   //----------------------------------------------------------------------------
   // Loop

   bool tGoing=mRxSocket.mValidFlag;

   while(tGoing)
   {
      // Try to receive a message with a blocking receive call.
      // If a message was received then process it.
      // If a message was not received then the connection was lost.  
      ByteContent* tMsg=0;
      if (mRxSocket.doReceiveMsg(tMsg))
      {
         // Message was correctly received
         // Call the receive method
         processRxMsg(tMsg);
      }
      else
      {
         // Message was not correctly received
      }
      //-------------------------------------------------------------------------
      // If termination request, exit the loop
      // This is set by shutdown, see below.
      if (mTerminateFlag)
      {
         tGoing=false;
      }  
   }         
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function, base class overload.

void UdpMsgThread::threadExitFunction()
{
   Prn::print(Prn::SocketInit1, "UdpMsgThread::threadExitFunction");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Shutdown, base class overload.
// This sets the terminate request flag and closes the socket.
//
// If the while loop in the threadRunFunction is blocked on doReceiveMsg then
// closing the socket will cause doReceiveMsg to return with false and 
// then the terminate request flag will be polled and the threadRunFunction 
// will exit.

void UdpMsgThread::shutdownThread()
{
   BaseThreadWithTermFlag::mTerminateFlag = true;

   mRxSocket.doClose();

   BaseThreadWithTermFlag::waitForThreadTerminate();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Invoke the qcall callback.

void UdpMsgThread::processRxMsg(Ris::ByteContent* aMsg)
{
   // Invoke the receive qcall callback, passes the received message to the
   // thread owner.
   mRxMsgQCall(aMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This sends a message via the tcp client thread

void UdpMsgThread::sendMsg (Ris::ByteContent* aMsg)
{
   mTxSocket.doSendMsg(aMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

