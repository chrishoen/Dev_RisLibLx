/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "someThreadParms.h"

#define  _SOMETESTTHREAD_CPP_
#include "someTestThread.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

TestThread::TestThread()
{
   // Set base class variables.
   BaseClass::setThreadName("Test");
   BaseClass::setThreadPrintLevel(TS::PrintLevel(5, 3));
   BaseClass::setThreadPriorityHigh();

   mTimerPeriod = gThreadParms.mTimerThreadPeriod;
   mTimerPeriod = 0;
   mTimerPeriod = 4000;
   mTPFlag = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immediately 
// after the thread starts running.

void TestThread::threadInitFunction()
{
   mWaitable.initialize(mTimerPeriod);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function.

void TestThread::threadRunFunction()
{
   // Loop to wait for posted events and process them.
   int tCount = 0;
   while (true)
   {
      // Wait for the timer or the semaphore.
      mWaitable.waitForTimerOrSemaphore();
      // Test for thread termination.
      if (BaseClass::mTerminateFlag) break;

      // Call a handler for the timer or the semaphore.
      if (mWaitable.wasTimer()) executeOnTimer(tCount++);
      if (mWaitable.wasSemaphore()) executeOnSemaphore();
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immediately
// before the thread is terminated.

void TestThread::threadExitFunction()
{
   mWaitable.finalize();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread shutdown function. Set the termination flag, post to the 
// waitable semaphore and wait for the thread to terminate.

void TestThread::shutdownThread()
{
   BaseClass::shutdownThreadPrologue();

   BaseClass::mTerminateFlag = true;
   mWaitable.postSemaphore();
   BaseClass::waitForThreadTerminate();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TestThread::executeOnTimer(int aCount)
{
   if (!mTPFlag) return;
   Prn::print(Prn::View11, "executeOnTimer %d", aCount);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TestThread::executeOnSemaphore()
{
   Prn::print(Prn::View11, "executeOnSemaphore");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace