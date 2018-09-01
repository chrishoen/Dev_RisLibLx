#pragma once

/*==============================================================================
Timer thread base class
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risThreadsThreads.h"

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is an extension of the virtual base class for general purpose threads.
// It provides a basis for a periodic thread that uses a polling termination
// policy.

class  BaseTimerThread : public BaseThread
{
public:

   typedef Ris::Threads::BaseThread BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // The thread run function contains a loop that waits on this semaphore
   // and times out after the timer period variable.
   Ris::Threads::BinarySemaphore mSemaphore;

   // Timer period, milliseconds.
   int   mTimerPeriod;

   // Timer count incremented at each timer execution.
   int   mTimerCount;

   // If true then the thread run function loop terminates.
   bool  mTerminateFlag;
   
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Intrastructure.

   // Constructor.
   BaseTimerThread(); 

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Base class override.
   // Execute an infinite loop that sleeps for the timer period and 
   // periodically calls the inheritor executeOnTimer. The sleep is obtained
   // by waiting on the semaphore member and timing out. The loop exits when the
   // termination flag is true.
   void threadRunFunction() override;

   // Base class override.
   // Set the termination flag and post to the semaphore member.
   void shutdownThread()override;

   // An overload of this is supplied by the inheritor.
   // It is called periodically by the threadRunFunction
   virtual void executeOnTimer(int aTimerCount)=0;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace


