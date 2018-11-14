//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "prnPrint.h"

#include "ris_priorities.h"
#include "risThreadsQCallThread.h"

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

BaseQCallThread::BaseQCallThread()
{
   mThreadPriority = get_default_qcall_thread_priority();

   mTerminateFlag = false;
   mCallQueSize=1000;
   mQCallAbortFlag = false;

   mTimerPeriod = 1000;
   mCurrentTimeCount = 0;
}

BaseQCallThread::~BaseQCallThread()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread resource init function. This is called by the base class
// after the thread starts running. It initializes the call queue.

void BaseQCallThread::threadResourceInitFunction()
{
   // Initialize the call queue.
   BaseQCallTarget::initializeCallQueue(mCallQueSize);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread timer init function. This is called by the base class
// after the thread starts running. It initializes the timer.

void BaseQCallThread::threadTimerInitFunction()
{
   mWaitable.initialize(mTimerPeriod);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function. This is called by the base class immediately 
// after the thread init function. It runs a loop that waits for the
// waitable timer or event.
// 
// This provides the execution context for processing queued qcalls
// It waits for the call queue waitable event, extracts a call from
// the call queue, and executes the call.
//
// When a qcall is written to the call queue, this wakes up, reads it from the 
// queue, and executes it in the context of this thread.
//
// When the periodic waitable timer expires, this wakes up and executes 
// the inheritors timer function in the context of this thread.

void BaseQCallThread::threadRunFunction()
{
   // Loop to process the call queue or timer.
   // Exit the loop on a thread terminate.
   while (true)
   {
      // Wait for a timer or an event.
      mWaitable.waitForTimerOrEvent();

      // Test for thread termination.
      if (mTerminateFlag) break;

      // If the periodic timer occurred.
      if (mWaitable.wasTimer())
      {
         // Call the inheritors handler for the timer.
         executeOnTimer(mCurrentTimeCount++);
      }

      // If an event was posted.
      if (mWaitable.wasEvent())
      {
         // Try to read a qcall from the queue.
         BaseQCall* tQCall = (BaseQCall*)mCallQueue.tryRead();

         // Test the read.
         if (tQCall)
         {
            // Set the abort flag false for each qcall.
            mQCallAbortFlag = false;
            // Execute the qcall.
            tQCall->execute();
            // Delete the qcall.
            delete tQCall;
         }
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread timer exit function. This is called by the base class
// before the thread is terminated. It finalizes the timer.

void BaseQCallThread::threadTimerExitFunction()
{
   mWaitable.finalize();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread resource exit function. This is called by the base class
// before the thread is terminated. It finalizes the call queue.

void BaseQCallThread::threadResourceExitFunction()
{
   BaseQCallTarget::finalizeCallQueue();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Shutdown the thread.

void BaseQCallThread::shutdownThread()
{
   // Set the termination flag.
   mTerminateFlag = true;
   // Post to the waitable event. This will wake up the threadRunFunction
   // and it will return because the termination flag was set.
   mWaitable.postEvent();
   // Wait for the thread run function to return..
   waitForThreadTerminate();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Post to the waitable event. It is called by qcall invocations after
// a qcall has been enqueued to the call queue. It wakes up the thread run
// function to process the call queue.

void BaseQCallThread::notifyQCallAvailable()
{
   mWaitable.postEvent();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace
