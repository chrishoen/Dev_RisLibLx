//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "prnPrint.h"

#include "risThreadsTimerThread.h"

namespace Ris
{
namespace Threads
{
//******************************************************************************
//******************************************************************************
//******************************************************************************

BaseTimerThread::BaseTimerThread()
{
   mTimerPeriod = 1000;
   mTimerCount = 0;
   mTerminateFlag = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread resource init function. This is called by the base class
// after the thread starts running. It initializes the waitable timer.

void BaseTimerThread::threadResourceInitFunction()
{
   mWaitable.initialize(mTimerPeriod);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function. This is called by the base class immediately 
// after the thread init function. It runs a loop that waits for the
// timer or the termination event.

void BaseTimerThread::threadRunFunction()
{
   // Loop to wait for posted events and process them.
   while (true)
   {
      // Wait for a timer or an event.
      mWaitable.waitForTimerOrSemaphore();

      // Test for thread termination.
      if (mTerminateFlag) break;

      // Call the inheritors handler for the timer.
      if (mWaitable.wasTimer()) executeOnTimer(mTimerCount++);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread resource exit function. This is called by the base class
// before the thread is terminated. It finalizes the waitable timer.

void BaseTimerThread::threadResourceExitFunction()
{
   mWaitable.finalize();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread shutdown function. Set the termination flag, post to the 
// waitable event and wait for the thread to terminate.

void BaseTimerThread::shutdownThread()
{
   shutdownThreadPrologue();
   mTerminateFlag = true;
   mWaitable.postSemaphore();
   BaseClass::waitForThreadTerminate();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

