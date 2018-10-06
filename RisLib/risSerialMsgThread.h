#pragma once

/*==============================================================================
Serial port message thread.
==========================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risThreadsThreads.h"
#include "risThreadsQCall.h"
#include "risByteContent.h"
#include "risByteMsgMonkey.h"
#include "risSerialMsgPort.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Serial message thread.
//
// This is a thread that provides the execution context for byte content
// message communication via a serial port.
//
// It contains a serial message port.
//
// The data that is communicated via the serial port is encapsulated according
// to the byte content messaging scheme. It sends and receives byte content
// messages.
//
// The thread is structured around a while loop that does a read call to
// receive a message on the serial port.
//
// The thread provides serialized access to the serial port and associated 
// state variables and it provides the context for the blocking of the 
// read call.
//
// An instance of this thread is created as a child thread of a parent thread
// that performs message processing. The parent creates the child and
// registers a receive message qcall callback to it. When the child thread
// receives a message it invokes the message qcall to pass it to the parent
// for processing.

class SerialMsgThread : public Ris::Threads::BaseThreadWithTermFlag
{
public:
   typedef Ris::Threads::BaseThreadWithTermFlag BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Serial settings
   SerialSettings mSettings;

   // Serial message port.
   SerialMsgPort mSerialMsgPort;

   // This is a qcall callback that is called when a message is received.
   // It is registered by the parent thread at initialzation.
   typedef Ris::Threads::QCall1<Ris::ByteContent*> RxMsgQCall;
   RxMsgQCall mRxMsgQCall;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Metrics.
   int mTxCount;
   int mTxLength;
   int mRxCount;
   int mRxError;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   SerialMsgThread(SerialSettings& aSettings);
   ~SerialMsgThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods, thread base class overloads.

   // Initialize and open the serial port.
   void threadInitFunction()override;

   // Execute a while loop that does receive calls. The loop exits
   // when the serial port is closed and the termination flag is true.
   void threadRunFunction()override;

   // Print.
   void threadExitFunction()override;

   // Set the termination flag, close the serial port and wait for the thread
   // to terminate.
   void shutdownThread()override; 

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Pass a received message to the parent thread. This is called by the
   // threadRunFunction when a message is received. It invokes the
   // mRxMsgQCall that is registered at initialization.
   virtual void processRxMsg(Ris::ByteContent* aMsg);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Send a transmit message through the socket to the peer. It executes a
   // blocking send call in the context of the calling thread. It is protected
   // by a mutex semaphore.
   void sendMsg (Ris::ByteContent* aMsg);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

