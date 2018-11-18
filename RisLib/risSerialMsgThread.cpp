/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "my_functions.h"
#include "risPortableCalls.h"
#include "prnPrint.h"

#include "risThreadsPriorities.h"
#include "risSerialMsgThread.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

SerialMsgThread::SerialMsgThread(SerialSettings& aSettings)
{
   // Set base class thread services.
   BaseClass::setThreadName("SerialMsg");
   BaseClass::setThreadPriority(aSettings.mThreadPriority);
   BaseClass::setThreadPrintLevel(aSettings.mPrintLevel);

   mSettings = aSettings;
   mRxMsgQCall = aSettings.mRxMsgQCall;

   mTxCount = 0;
   mTxLength = 0;
   mRxCount = 0;
   mRxError = 0;
}

SerialMsgThread::~SerialMsgThread()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function, base class overload.
// Initialize and open the serial port.

void SerialMsgThread::threadInitFunction()
{
   // Initialize and open the serial port.
   mSerialMsgPort.initialize(mSettings);
   mSerialMsgPort.doOpen();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function, base class overload.
// It contains a while loop that receives messages and passes them to the
// attached message handler.

void  SerialMsgThread::threadRunFunction()
{
   bool tGoing=mSerialMsgPort.mValidFlag;
 
   while(tGoing)
   {
      // Try to receive a message with a blocking receive call.
      // If a message was received then process it.
      // If a message was not received then the serial port was closed or 
      // an error occurred.  
      ByteContent* tMsg=0;
      if (mSerialMsgPort.doReceiveMsg(tMsg))
      {
         // Metrics.
         mRxCount++;
         // Message was correctly received.
         // Invoke the receive qcall callback, passing the received message
         // to the thread owner.
         processRxMsg(tMsg);
      }
      else
      {
         // Message was not correctly received.
         mRxError++;
      }

      // If termination request, exit the loop.
      // This is set by shutdown, see below.
      if (mTerminateFlag)
      {
         tGoing = false;
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function, base class overload.

void SerialMsgThread::threadExitFunction()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Shutdown, base class overload.
// This sets the terminate request flag and closes the serial port.
//
// If the while loop in the threadRunFunction is blocked on doReceiveMsg then
// closing the serial port will cause doReceiveMsg to return with false and 
// then the terminate request flag will be polled and the threadRunFunction 
// will exit.

void SerialMsgThread::shutdownThread()
{
   shutdownThreadPrologue();
   BaseThreadWithTermFlag::mTerminateFlag = true;

   mSerialMsgPort.doClose();

   BaseThreadWithTermFlag::waitForThreadTerminate();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Pass a received message to the parent thread. This is called by the
// threadRunFunction when a message is received. It invokes the
// mRxMsgQCall that is registered at initialization.

void SerialMsgThread::processRxMsg(Ris::ByteContent* aMsg)
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

void SerialMsgThread::sendMsg (Ris::ByteContent* aMsg)
{
   mSerialMsgPort.doSendMsg(aMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

