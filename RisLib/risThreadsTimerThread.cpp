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
// Thread timer init function. This is called by the base class
// after the thread starts running. It initializes the timer.

void BaseTimerThread::threadTimerInitFunction()
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
      mWaitable.waitForTimerOrEvent();

      // Test for thread termination.
      if (mTerminateFlag) break;

      // Call the inheritors handler for the timer.
      if (mWaitable.wasTimer()) executeOnTimer(mTimerCount++);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class
// before the thread is terminated. It finalizes the timer.

void BaseTimerThread::threadTimerExitFunction()
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
   mTerminateFlag = true;
   mWaitable.postEvent();
   BaseClass::waitForThreadTerminate();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

