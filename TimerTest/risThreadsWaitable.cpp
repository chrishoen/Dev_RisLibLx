/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <sys/timerfd.h>
#include <sys/eventfd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#include "risThreadsThreads.h"
#include "prnPrint.h"

#include "risThreadsWaitable.h"

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// class definition for implementation specific

class Waitable::Specific
{
public:
   int mTimerFd;
   int mEventFd;

   Specific()
   {
      mTimerFd = 0;
      mEventFd = 0;
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

Waitable::Waitable()
{
   // Initialize members
   mTimerPeriod=1000;

   // Create new specific implementation
   mSpecific = new Specific;
}


Waitable::~Waitable()
{
   finalize();
   delete mSpecific;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Start a timer call periodically, in milliseconds

void Waitable::initialize (int aTimerPeriod)
{
   // Guard.
   int tTimerPeriod = aTimerPeriod;
   if (aTimerPeriod == 0) tTimerPeriod = 1000*1000*1000;

   // Create the timer.
   mSpecific->mTimerFd = timerfd_create(CLOCK_REALTIME, 0);

   if (mSpecific->mTimerFd <= 0)
   {
      TS::print(0, "timerfd_create_error_1 %d", errno);
      return;
   }

   // Calculate the timer interval.
   int tSec = tTimerPeriod / 1000;
   int tMs = (tTimerPeriod % 1000);
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

   // Create the event.
   mSpecific->mEventFd = eventfd(0, EFD_SEMAPHORE);
   if (mSpecific->mEventFd <= 0)
   {
      TS::print(0, "eventfd_create_error_1 %d", errno);
      return;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Waitable::finalize()
{
   if (mSpecific->mTimerFd == 0) return;
   
   int tRet;
   tRet = close(mSpecific->mTimerFd);
   if (tRet == -1)
   {
      TS::print(0, "timer close error_1 %d", errno);
   }

   tRet = close(mSpecific->mEventFd);
   if (tRet == -1)
   {
      TS::print(0, "event close error_1 %d", errno);
   }

   mSpecific->mTimerFd = 0;
   mSpecific->mEventFd = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Wait for timer or event.

void Waitable::waitForTimerOrEvent()
{
   TS::print(0, "");
   TS::print(0, "Waitable waitForTimerOrEvent*******************************BEGIN");

   // Add the timer and event fds to a read set.
   int tRet;
   fd_set  tReadSet;
   FD_ZERO(&tReadSet);
   FD_SET(mSpecific->mTimerFd, &tReadSet);
   FD_SET(mSpecific->mEventFd, &tReadSet);

   // Select on the readset. This blocks until one of the handles is readable.
   TS::print(0, "Waitable wait select");
   tRet = select(FD_SETSIZE, &tReadSet, 0, 0, 0);
   if (tRet == -1)
   {
      TS::print(0, "Waitable select read error_1 %d", errno);
   }

   // Test if the timer is ready to be read.
   if (FD_ISSET(mSpecific->mTimerFd, &tReadSet))
   {
      // Read the timer. Because of the select this should not be blocked.
      TS::print(0, "Waitable wait read timer");
      unsigned long long tExpired = 0;
      tRet = (int)read(mSpecific->mTimerFd, &tExpired, sizeof(tExpired));
      if (tRet == -1)
      {
         TS::print(0, "Waitable read timer error_1 %d", errno);
      }
   }

   // Test if the event is ready to be read.
   if (FD_ISSET(mSpecific->mEventFd, &tReadSet))
   {
      // Read the event. Because of the select this should not be blocked.
      TS::print(0, "Waitable wait read event>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
      unsigned long long tValue = 0;
      tRet = (int)read(mSpecific->mEventFd, &tValue, sizeof(tValue));
      if (tRet == -1)
      {
         TS::print(0, "Waitable read event error_1 %d", errno);
      }
   }

   TS::print(0, "Waitable waitForTimerOrEvent*******************************END");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Post to the event. This will unblock any pending waits.

void Waitable::postEvent()
{
   int tRet;
   unsigned long long tValue = 1;

   // Write to the event semaphore, increment by one.
   tRet = (int)write(mSpecific->mEventFd, &tValue, sizeof(tValue));
   if (tRet == -1)
   {
      TS::print(0, "Waitable post event error_1 %d", errno);
   }

}
//******************************************************************************
//******************************************************************************
//******************************************************************************

}//namespace
}//namespace

