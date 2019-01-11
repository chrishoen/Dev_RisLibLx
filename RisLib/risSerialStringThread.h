#pragma once

/*==============================================================================
Serial port string thread.
==========================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <string>

#include "risThreadsThreads.h"
#include "risThreadsQCall.h"
#include "risSerialPort.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Serial string thread.
//
// This is a thread that provides the execution context for 
// string communication via a serial port.
//
// It contains a serial port.
//
// The data that is communicated via the serial port is encapsulated according
// to the strings that are terminated with a newline. It sends and receives
// strings.
//
// The thread is structured around a while loop that does a read call to
// receive a string on the serial port.
//
// The thread provides serialized access to the serial port and associated 
// state variables and it provides the context for the blocking of the 
// read call.
//
// An instance of this thread is created as a child thread of a parent thread
// that performs string processing. The parent creates the child and
// registers a receive string qcall callback to it. When the child thread
// receives a string it invokes the string qcall to pass it to the parent
// for processing.

class SerialStringThread : public Ris::Threads::BaseThreadWithTermFlag
{
public:
   typedef Ris::Threads::BaseThreadWithTermFlag BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants.

   static const int cMaxStringSize = 200;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Serial settings
   SerialSettings mSettings;

   // Serial string port.
   SerialPort mSerialPort;

   // Rx string.
   char mRxString[cMaxStringSize];

   // This is a qcall callback that is called when a string is received.
   // It is registered by the parent thread at initialzation.
   typedef Ris::Threads::QCall1<std::string*> RxStringQCall;
   RxStringQCall mRxStringQCall;

   // This is a function pointer callback that is called when a string is
   //  received. It is registered by the parent thread at initialzation.
   std::function<void(std::string*)> mRxStringCallback;

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
   SerialStringThread(SerialSettings& aSettings);
   ~SerialStringThread();

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

   // Pass a received string to the parent thread. This is called by the
   // threadRunFunction when a string is received. It invokes the
   // mRxStringQCall that is registered at initialization.
   void processRxString(char* aString);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Send a null terminated string via the serial port. A newline terminator
   // is appended to the string before transmission. This executes in the
   // context of the The calling thread.
   void sendString(const char* aString);

   // Send a null terminated string via the serial port. A newline terminator
   // is appended to the string before transmission. This executes in the
   // context of the The calling thread. The string is deleted after
   // transmission.
   void sendString(std::string* aString);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

