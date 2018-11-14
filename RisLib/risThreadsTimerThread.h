#pragma once

/*==============================================================================
Timer thread base class
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risThreadsThreads.h"
#include "risThreadsWaitable.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

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

   // Waitable timer or event. The thread run function contains a loop that
   // waits on this timer.
   Ris::Threads::Waitable mWaitable;

   // Timer period, milliseconds. Inheritors should set this in their
   // constructors. 
   int   mTimerPeriod;

   // Timer count incremented at each timer execution.
   int   mTimerCount;

   // If true then the thread run function loop terminates.
   bool  mTerminateFlag;
   
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   BaseTimerThread(); 

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Base class overloads.

   // Thread timer init function. This is called by the base class
   // after the thread starts running. It initializes the timer.
   void threadTimerInitFunction() override;

   // Thread run function. This is called by the base class immediately 
   // after the thread init function. It runs a loop that waits for the
   // timer or the termination event.
   void threadRunFunction() override;

   // Thread exit function. This is called by the base class
   // before the thread is terminated. It finalizes the timer.
   void threadTimerExitFunction() override;

   // Thread shutdown function. Set the termination flag, post to the 
   // waitable event and wait for the thread to terminate.
   void shutdownThread() override;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // An overload of this is supplied by the inheritor.
   // It is called periodically by the threadRunFunction
   virtual void executeOnTimer(int aTimerCount)=0;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace


