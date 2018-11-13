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

   mTPFlag = true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immediately 
// after the thread starts running.

void TestThread::threadInitFunction()
{
   mTimer.startTimer(1000);
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
      // Test for thread termination.
      if (BaseClass::mTerminateFlag) break;

      // Wait for the timer.
      mTimer.waitForTimer();

      // Send a qcall to the test thread.
      if (mTPFlag) executeOnTimer(tCount);
      tCount++;
   }
}
//BaseClass::threadSleep(1000);

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immediately
// before the thread is terminated.

void TestThread::threadExitFunction()
{
   mTimer.stopTimer();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TestThread::executeOnTimer(int aCount)
{
   Prn::print(0, "executeOnTimer %d", aCount);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace