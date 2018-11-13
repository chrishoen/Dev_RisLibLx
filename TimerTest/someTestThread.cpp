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

   mTPFlag = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immediately 
// after the thread starts running.

void TestThread::threadInitFunction()
{
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

      // Wait for a random delay.
      BaseClass::threadSleep(1000);

      // Send a qcall to the test thread.
      if (mTPFlag) executeOnTimer(tCount);
      tCount++;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immediately
// before the thread is terminated.

void TestThread::threadExitFunction()
{
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