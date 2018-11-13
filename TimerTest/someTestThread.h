#pragma once

/*==============================================================================
Some timer thread class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risThreadsThreads.h"
#include "risThreadsWaitableTimer.h"

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
   Ris::Threads::WaitableTimer mTimer;

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

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. 

   void executeOnTimer(int aCount);
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
