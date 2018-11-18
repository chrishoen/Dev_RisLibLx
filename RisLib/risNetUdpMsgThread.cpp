/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "prnPrint.h"

#include "risThreadsPriorities.h"
#include "risNetUdpMsgThread.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

UdpMsgThread::UdpMsgThread(Settings& aSettings)
{
   // Base class variables.
   BaseClass::setThreadName("UdpMsg");
   BaseClass::setThreadPriority(aSettings.mThreadPriority);
   BaseClass::setThreadPrintLevel(aSettings.mPrintLevel);

   // Store settings.
   mSettings = aSettings;
   mRxMsgQCall = aSettings.mRxMsgQCall;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function, base class overload.
// It configures the socket.

void UdpMsgThread::threadInitFunction()
{
   // Initialize and configure the sockets.
   mRxSocket.initialize(mSettings);
   mRxSocket.configure();
   mTxSocket.initialize(mSettings);
   mTxSocket.configure();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function, base class overload.
// It contains a while loop that receives messages.

void  UdpMsgThread::threadRunFunction()
{
   bool tGoing=mRxSocket.mValidFlag;

   while(tGoing)
   {
      // Try to receive a message with a blocking receive call.
      // If a message was received then process it.
      ByteContent* tMsg=0;
      if (mRxSocket.doReceiveMsg(tMsg))
      {
         // Message was correctly received.
         // Call the receive callback qcall.
         processRxMsg(tMsg);
      }
      else
      {
         // Message was not correctly received.
      }

      // If termination request then exit the loop.
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
   shutdownThreadPrologue();
   BaseThreadWithTermFlag::mTerminateFlag = true;

   mRxSocket.doClose();

   BaseThreadWithTermFlag::waitForThreadTerminate();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Pass a received message to the parent thread. This is called by the
// threadRunFunction when a message is received. It invokes the
// mRxMsgQCall that is registered at initialization.

void UdpMsgThread::processRxMsg(Ris::ByteContent* aMsg)
{
   // Guard.
   if (!mRxMsgQCall.isValid()) return;

   // Invoke the receive callback qcall.
   mRxMsgQCall(aMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a transmit message through the socket to the peer. It executes a
// blocking send call in the context of the calling thread. It is protected
// by a mutex semaphore.

void UdpMsgThread::sendMsg (Ris::ByteContent* aMsg)
{
   mTxSocket.doSendMsg(aMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

