//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "prnPrint.h"

#include "risThreadsPriorities.h"
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
   setThreadPrintLevel(TS::PrintLevel(0, 3));
   setThreadPriority(Ris::Threads::gPriorities.mQCall);

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
// after the thread starts running. It initializes the call queue and
// the waitable timer.

void BaseQCallThread::threadResourceInitFunction()
{
   // Initialize the call queue.
   mCallQueue.initialize(mCallQueSize);

   // Initialize the waitable timer.
   mWaitable.initialize(mTimerPeriod);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function. This is called by the base class immediately 
// after the thread init function. It runs a loop that waits for the
// waitable timer or semaphore.
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
      // Wait for the timer or the counting semaphore.
      mWaitable.waitForTimerOrSemaphore();

      // Test for thread termination.
      if (mTerminateFlag) break;

      // If the periodic timer was signaled.
      if (mWaitable.wasTimer())
      {
         // Call the inheritors timer handler.
         executeOnTimer(mCurrentTimeCount++);
      }

      // If the counting semaphore was signaled.
      if (mWaitable.wasSemaphore())
      {
         // Try to read a qcall from the queue.
         if (BaseQCall* tQCall = (BaseQCall*)mCallQueue.tryRead())
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
// Thread resource exit function. This is called by the base class
// before the thread is terminated. It finalizes the call queue and
// the waitable timer.

void BaseQCallThread::threadResourceExitFunction()
{
   // Finalize the call queue.
   mCallQueue.finalize();

   // Finalize the waitable timer;
   mWaitable.finalize();

}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Shutdown the thread.

void BaseQCallThread::shutdownThread()
{
   shutdownThreadPrologue();
   // Set the termination flag.
   mTerminateFlag = true;
   // Post to the waitable semaphore. This will wake up the threadRunFunction
   // and it will return because the termination flag was set.
   mWaitable.postSemaphore();
   // Wait for the thread run function to return.
   waitForThreadTerminate();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Try to write a qcall to the to the target queue. Return true if
// successful. This is called by qcall invocations to enqueue a qcall.
// It writes to the call queue and posts to the waitable semaphore, which
// then wakes up the thread run function to process the call queue.

bool BaseQCallThread::tryWriteQCall(BaseQCall* aQCall) 
{
   // Guard.
   if (mTerminateFlag) return false;

   // Try to write to the call queue.
   if (!mCallQueue.tryWrite(aQCall))
   {
      // The write was not successful.
      return false;
   }

   // Post to the waitable semaphore.
   mWaitable.postSemaphore();

   // Successful.
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace
