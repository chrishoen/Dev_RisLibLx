#pragma once

/*==============================================================================
Thread Services.
These provide printing, logging, and error handling services that are
based on having the controlling parameters located in thread local storage.

print thread class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdio.h>

#include "risThreadsQCallThread.h"
#include "tsPrintString.h"

namespace TS
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a qcall thread that prints strings to the console and the log file.
//   
// It inherits from BaseQCallThread to obtain a call queue based thread
// functionality.

class  PrintThread : public Ris::Threads::BaseQCallThread
{
public:
   typedef Ris::Threads::BaseQCallThread BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants.

   static const int cMaxStringSize = 400;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Log file descriptor.
   FILE* mFile;

   // If true then prints are enabled.
   bool mEnableFlag;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Metrics.
   int  mWriteCount;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   PrintThread();
  ~PrintThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Thread base class overloads.

   // Thread init function. This is called by the base class immediately 
   // after the thread starts running.
   void threadInitFunction() override;

   // Thread exit function. This is called by the base class immediately
   // before the thread is terminated.
   void threadExitFunction() override;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Write qcall.
   Ris::Threads::QCall1<PrintString*> mWriteQCall;

   // Write function. This is bound to the qcall. It writes a string to the
   // log file and then deletes it
   void executeWrite (PrintString* aString);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // File open and close.
   void doFileOpenNew();
   void doFileOpenAppend();
   void doFileClose();
   void doFileFlush();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global singular instance.

#ifdef _TSPRINTTHREAD_CPP_
         PrintThread* gPrintThread = 0;
#else
extern   PrintThread* gPrintThread;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
