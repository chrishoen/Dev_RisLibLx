/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <sys/timerfd.h>
#include <sys/eventfd.h>
#include <sys/types.h>
//#include <sys/socket.h>
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
// Class definition for implementation specific.

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
   // Initialize members.
   mTimerCount = 0;
   mWasTimerFlag = false;
   mWasEventFlag = false;
   mValidFlag = false;

   // Create new specific implementation.
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
   // Initialize variables.
   mTimerCount = 0;
   mWasTimerFlag = false;
   mWasEventFlag = false;

   // If using the timer.
   if (aTimerPeriod > 0)
   {
      // Create the timer.
      mSpecific->mTimerFd = timerfd_create(CLOCK_REALTIME, 0);

      // Calculate the timer interval.
      int tSec = aTimerPeriod / 1000;
      int tMs = (aTimerPeriod % 1000);
      int tNs = tMs * 1000 * 1000;
      struct itimerspec tNewValue;
      tNewValue.it_value.tv_sec = tSec;
      tNewValue.it_value.tv_nsec = tNs;
      tNewValue.it_interval.tv_sec = tSec;
      tNewValue.it_interval.tv_nsec = tNs;

      // Set the timer interval.
      timerfd_settime(mSpecific->mTimerFd, 0, &tNewValue, NULL);
   }
   // If not using the timer.
   else
   {
      mSpecific->mTimerFd = -1;
   }

   // Create the event.
   mSpecific->mEventFd = eventfd(0, EFD_SEMAPHORE);

   // Set valid.
   mValidFlag = true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Waitable::finalize()
{
   // Guard.
   if (!mValidFlag) return;

   // Set invalid.
   mValidFlag = false;
   
   // Close the timer and event.
   close(mSpecific->mTimerFd);
   close(mSpecific->mEventFd);

   mSpecific->mTimerFd = 0;
   mSpecific->mEventFd = 0;

}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Wait for timer or event.

void Waitable::waitForTimerOrEvent()
{
   // Reset variables.
   mWasTimerFlag = false;
   mWasEventFlag = false;

   // Guard.
   if (!mValidFlag) return;

   TS::print(5, "Waitable waitForTimerOrEvent*******************************BEGIN");

   // Add the timer and event fds to a read set.
   int tRet;
   fd_set  tReadSet;
   FD_ZERO(&tReadSet);
   FD_SET(mSpecific->mTimerFd, &tReadSet);
   FD_SET(mSpecific->mEventFd, &tReadSet);

   // Select on the readset. This blocks until one of the handles is readable.
   TS::print(5, "Waitable wait select");
   select(FD_SETSIZE, &tReadSet, 0, 0, 0);

   // Test if the timer is ready to be read.
   if (FD_ISSET(mSpecific->mTimerFd, &tReadSet))
   {
      // Read the timer. Because of the select this should not be blocked.
      TS::print(5, "Waitable wait read timer");
      unsigned long long tExpired = 0;
      read(mSpecific->mTimerFd, &tExpired, sizeof(tExpired));
      
      // Set the flag.
      mTimerCount++;
      mWasTimerFlag = true;
   }

   // Test if the event is ready to be read.
   if (FD_ISSET(mSpecific->mEventFd, &tReadSet))
   {
      // Read the event. Because of the select this should not be blocked.
      TS::print(5, "Waitable wait read event>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
      unsigned long long tValue = 0;
      read(mSpecific->mEventFd, &tValue, sizeof(tValue));
      // Set the flag.
      mWasEventFlag = true;
   }

   TS::print(5, "Waitable waitForTimerOrEvent*******************************END");
}
//******************************************************************************
//******************************************************************************
//******************************************************************************

bool Waitable::wasTimer() { return mWasTimerFlag; }
bool Waitable::wasEvent() { return mWasEventFlag; }

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Post to the event. This will unblock any pending waits.

void Waitable::postEvent()
{
   // Guard.
   if (!mValidFlag) return;

   // Write to the event semaphore, increment by one.
   unsigned long long tValue = 1;
   write(mSpecific->mEventFd, &tValue, sizeof(tValue));
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

