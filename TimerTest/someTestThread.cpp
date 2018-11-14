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
   BaseClass::setThreadPrintLevel(3);
   BaseClass::setThreadLogLevel(3);

   mTimerPeriod = 1000;
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
// Thread run function. This is called by the base class immediately 
// after the thread init function. It runs a loop that waits on SDL
// events and processes posted events. The loop exits when it receives
// a quit event.

void TestThread::threadRunFunction()
{
   // Loop to wait for posted events and process them.
   int tCount = 0;
   while (true)
   {
      // Wait for a timer or an event.
      mWaitable.waitForTimerOrEvent();

      // Test for thread termination.
      if (BaseClass::mTerminateFlag) break;

      // Call a handler for the timer or the event.
      if (mWaitable.wasTimer()) executeOnTimer(tCount++);
      if (mWaitable.wasEvent()) executeOnEvent();
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
// waitable event and wait for the thread to terminate.

void TestThread::shutdownThread()
{
   BaseClass::mTerminateFlag = true;
   mWaitable.postEvent();
   BaseClass::waitForThreadTerminate();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TestThread::executeOnTimer(int aCount)
{
   if (!mTPFlag) return;
   Prn::print(0, "executeOnTimer %d", aCount);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TestThread::executeOnEvent()
{
   Prn::print(0, "executeOnEvent");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace