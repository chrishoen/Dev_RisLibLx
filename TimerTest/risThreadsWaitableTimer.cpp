/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <sys/timerfd.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "risThreadsThreads.h"
#include "prnPrint.h"

#include "risThreadsWaitableTimer.h"

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// class definition for implementation specific

class WaitableTimer::Specific
{
public:
   int mTimerFd;
   Specific()
   {
      mTimerFd = 0;
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

WaitableTimer::WaitableTimer()
{
   // Initialize members
   mTimerPeriod=1000;

   // Create new specific implementation
   mSpecific = new Specific;
}


WaitableTimer::~WaitableTimer()
{
   stopTimer();
   delete mSpecific;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Start a timer call periodically, in milliseconds

void WaitableTimer::startTimer (int aTimerPeriod)
{
   // Guard.
   if (aTimerPeriod == 0) return;

   // Create the timer.
   mSpecific->mTimerFd = timerfd_create(CLOCK_REALTIME, 0);

   if (mSpecific->mTimerFd <= 0)
   {
      TS::print(0, "timerfd_create_error_1 %d", errno);
      return;
   }

   // Calculate the timer interval.
   int tSec = aTimerPeriod / 1000;
   int tMs = (aTimerPeriod % 1000);
   int tNs = tMs * 1000*1000;
   struct itimerspec tNewValue;
   tNewValue.it_value.tv_sec = tSec;
   tNewValue.it_value.tv_nsec = tNs;
   tNewValue.it_interval.tv_sec = tSec;
   tNewValue.it_interval.tv_nsec = tNs;

   // Set the timer interval.
   int tRet = timerfd_settime(mSpecific->mTimerFd, 0, &tNewValue, NULL);
   if (tRet == -1)
   {
      TS::print(0, "timerfd_settime_error_1 %d", errno);
      return;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void WaitableTimer::stopTimer()
{
   if (mSpecific->mTimerFd == 0) return;
   
   int tRet = close(mSpecific->mTimerFd);
   if (tRet == -1)
   {
      TS::print(0, "timer close error_1 %d", errno);
   }

   mSpecific->mTimerFd = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Wait for the timer.

void WaitableTimer::waitForTimer()
{
   unsigned long long tExpired = 0;
   int tRet = (int)read(mSpecific->mTimerFd, &tExpired, sizeof(tExpired));
   if (tRet == -1)
   {
      TS::print(0, "timer read error_1 %d", errno);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

}//namespace
}//namespace

