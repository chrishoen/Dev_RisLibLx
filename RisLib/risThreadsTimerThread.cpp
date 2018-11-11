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
   // Members
   mTerminateFlag = false;
   mTimerPeriod = countsPerOneSecond();
   mTimerCount  = 0;

   BaseClass::setThreadName("TimerThread");
   BaseClass::setThreadPrintLevel(0);

}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseTimerThread::threadRunFunction()
{
   while(1)
   {
      // Wait for semaphore to timeout
      mSemaphore.get(mTimerPeriod);
      // Test for termination
      if (mTerminateFlag) break;
      // Execute inheritor timer function
      executeOnTimer(mTimerCount++);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseTimerThread::shutdownThread()
{
   // Set terminate
   mTerminateFlag = true;
   mSemaphore.put();
   // Wait for terminate
   waitForThreadTerminate();
}   

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

