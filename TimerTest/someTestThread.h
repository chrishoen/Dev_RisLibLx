#pragma once

/*==============================================================================
Some timer thread class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risThreadsThreads.h"
#include "risThreadsWaitable.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a test timer thread that drives the test qcall thread.
//   

class TestThread : public Ris::Threads::BaseThreadWithTermFlag
{
public:

   typedef Ris::Threads::BaseThreadWithTermFlag BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Waitable timer.
   Ris::Threads::Waitable mWaitable;

   // Timer period.
   int mTimerPeriod;

   // If true then execute periodic function.
   bool mTPFlag;


   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   TestThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Thread base class overloads.

   // Thread init function. This is called by the base class immediately 
   // after the thread starts running.
   void threadInitFunction() override;

   // Thread run function. This is called by the base class immediately 
   // after the thread init function. It runs a loop that sends a qcall
   // to the qcall test thread and waits for a random time.
   void threadRunFunction() override;

   // Thread exit function. This is called by the base class immediately
   // before the thread is terminated.
   void threadExitFunction() override;

   // Thread shutdown function. Set the termination flag, post to the 
   // waitable event and wait for the thread to terminate.
   void shutdownThread() override;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. 

   // Test methods.
   void executeOnTimer(int aCount);
   void executeOnSemaphore();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global singular instance.

#ifdef _SOMETESTTHREAD_CPP_
         TestThread* gTestThread;
#else
extern   TestThread* gTestThread;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
