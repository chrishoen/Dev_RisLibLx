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

#include "risThreadsThreads.h"
#include "risThreadsSynch.h"
#include "risLCPointerQueue.h"
#include "tsPrintString.h"

namespace TS
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a thread class that prints strings to stdout and to a program 
// log file.
//   
// It contains a pointer queue and a counting semaphore. Threads that use
// this thread as a service to print strings write a string pointer to
// the queue and post to the semaphore. This thread waits on the semaphore
// and, when it wakes up, reads a string from the queue and prints it.
//
// It inherits from BaseThread to obtain thread functionality.
// 

class PrintThread : public Ris::Threads::BaseThread
{
public:
   typedef Ris::Threads::BaseThread BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants.

   static const int cMaxStringSize = 400;
   static const int cQueueSize = 1000;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Pointer queue that contains pointers to strings. Print invocations
   // enqueue strings to this queue. This thread dequeues strings from it and
   // prints them to the stdout and the program log file.
   Ris::LCPointerQueue mStringQueue;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Counting semaphore. The thread run function contains a loop that
   // waits on this for a timer or an event. The timer provides for
   // periodic exectution and the event can signify that the call queue is
   // ready or it can signify for thread termination.
   Ris::Threads::CountingSemaphore mSemaphore;

   // If true the the thread will terminate at the next post to the
   // semaphore.
   bool mTerminateFlag;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.


   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Log file descriptor.
   FILE* mFile;

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

   // Thread run function. This is called by the base class immediately 
   // after the thread init function. It runs a loop that waits on the
   // semaphore. When it wakes up, it reads a string from the string
   // queue and prints it.
   void threadRunFunction() override;

   // Thread exit function. This is called by the base class immediately
   // before the thread is terminated.
   void threadExitFunction() override;

   // Thread shutdown function. Set the termination flag, post to the 
   // semaphore and wait for the thread to terminate.
   void shutdownThread() override;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Print a string to stdout and the program log file and delete it.
   // log file and then deletes it. This is called by the thread run function
   // when it dequeues a string from the queue.
   void printString (PrintString* aString);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Try to write a string to the to the queue. Return true if
   // successful. This is called by print invocations to enqueue a string.
   // It writes to the string queue and posts to the semaphore, which
   // then wakes up the thread run function to process the string queue.
   bool tryWriteString(PrintString* aString);

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
