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

BaseQCallThread::BaseQCallThread()
{
   mThreadPriority = get_default_qcall_thread_priority();

   mTerminateFlag = false;
   mCallQueSize=1000;
   mQCallAbortFlag = false;

   mTimerExecuteFlag = false;
   mTimerPeriod = 1000;
   mCurrentTimeCount = 0;
   mTimerCurrentTimeCount = 0;
}

BaseQCallThread::~BaseQCallThread()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseQCallThread::threadResourceInitFunction()
{
   // Initialize the call queue.
   BaseQCallTarget::initializeCallQueue(mCallQueSize);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseQCallThread::threadTimerInitFunction()
{
   using namespace std::placeholders;

   // Guard.
   if (mTimerPeriod == 0) return;

   // Bind timer callback.
   mThreadTimerCall = std::bind (&BaseQCallThread::threadExecuteOnTimer, this, _1);

   // Start timer.
   mThreadTimer.startTimer(mThreadTimerCall,mTimerPeriod);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseQCallThread::threadExecuteOnTimer(int aCurrentTimeCount)
{
   // Update timer variables.
   mTimerCurrentTimeCount++;
   mTimerExecuteFlag=true;

   // Use central semaphore to wake up the thread.
   mCentralSem.put();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function, base class overload.
// This provides the execution context for processing queued QCalls
// It waits for the call queue semaphore, extracts a call from
// the call queue, and executes the call.
//
// When a QCall is written to the call queue, this wakes up, reads it from the 
// queue, and executes it. It returns true if successful, it returns false if
// mTerminateFlag was true.

void BaseQCallThread::threadRunFunction()
{
   // Loop to process the call queue.
   // Exit the loop on a thread terminate.
   Prn::print(Prn::QCallRun1, "BaseQCallThread::threadRunFunction");
   while (true)
   {
      // Wait for the central thread semaphore.
      mCentralSem.get();

      // Test for termination..
      if(mTerminateFlag) return;

      // Test for timer. 
      // When the timer service routine is called, it sets
      // TimerExecuteFlag true, increments mCurrentTimeCount, and
      // puts to mCentralSem. 
      if(mTimerExecuteFlag)
      {
         mTimerExecuteFlag=false;

         // Copy time count from timer.
         mCurrentTimeCount = mTimerCurrentTimeCount;

         // Execute inheritor timer function.
         executeOnTimer(mCurrentTimeCount);
      }

      // Not timer, the semaphore was from the thread call queue.
      else
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
// Thread init function, base class overload.

void BaseQCallThread::threadResourceExitFunction()
{
   Prn::print(Prn::QCallInit1, "BaseQCallThread::threadResourceExitFunction");

   // Finalize the call queue.
   BaseQCallTarget::finalizeCallQueue();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Shutdown the thread.

void BaseQCallThread::shutdownThread()
{
   Prn::print(Prn::QCallInit1, "BaseQCallThread::shutdownThread");

   // Set termination flag.
   mTerminateFlag=true;
   // Post to the call sem to wake up thread if blocked on it.
   mCentralSem.put();
   // Wait for thread terminate.
   waitForThreadTerminate();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Post to the central semahore. It is called by qcall invokations after
// a qcall has been enqueued to the call queue.

void BaseQCallThread::notifyQCallAvailable()
{
   mCentralSem.put();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace
