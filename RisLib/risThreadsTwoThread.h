#pragma once

/*==============================================================================
Two Thread, a thread construct that has a short term thread and a long term
thread. The short term thread executes qcalls that have a short duration
and the long term thread executes qcalls that can have long durations. Many
short term qcalls can execute while one long term qcall executes. It can be
used as a thread that can send a command somewhere and wait for a response.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <atomic>
#include "risLogic.h"
#include "risThreadsQCallThread.h"

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
//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a thread that executes short term qcalls. That is, execution of its
// thread qcalls is expected to take a short amount of time.

class TwoThreadShortThread : public Ris::Threads::BaseQCallThread
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants.

   // Returned status codes.
   enum
   {
      TimerCompletion_None        = 0,
      TimerCompletion_Timeout     = 1,
      TimerCompletion_Aborted     = 2,
      TimerCompletion_Forced      = 3,
      TimerCompletion_ForcedError = 4,
   };

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Thread call pointers, these are called by the thread base overloads, if 
   // they are bound. They are bound to functions by the instantiator before
   // the thread is launched. Any that are not bound result in a no op for the
   // thread run function.
   CallPointer0<>       mThreadInitCallPointer;
   CallPointer0<>       mThreadExitCallPointer;
   CallPointer1<char*>  mThreadExceptionCallPointer;
   CallPointer1<int>    mThreadExecuteOnTimerCallPointer;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Timer completion notification.
   // The above timer execution method posts to this semaphore,
   // if the down counter is not zero and then decrements to zero.
   BinarySemaphore  mTimerCompletionSem;
   std::atomic<int> mTimerCompletionDownCounter;

   // Timer completion code, set by above method
   int mTimerCompletionCode;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Intrastructure.

   // Constructor.
   TwoThreadShortThread();
  ~TwoThreadShortThread();

   // Thread init function. This is called by the base class immedidately 
   // after the thread starts running.
   // It calls the associated call pointer, if it exists.
   void threadInitFunction() override;

   // Thread exit function. This is called by the base class immedidately
   //  before the thread is terminated.
   // It calls the associated call pointer, if it exists.
   void threadExitFunction() override;

   // Thread exception function. This is called by the base class upon the
   // occurence of an execption.
   // It calls the associated call pointer, if it exists.
   void threadExceptionFunction(char* aStr) override;

   // Execute periodically. This is called by the base class timer.
   // It calls the associated call pointer, if it exists.
   void executeOnTimer(int aTimerCount) override;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Waits for a timer completion.
   int threadWaitForTimerCompletion(
      int aTimerCount); 

   // Abort any pending waits for timer completion.
   void threadAbortTimerCompletion();

   // Forces a timer completion.
   void threadForceTimerCompletion();

   // Forces a timer completion with an error.
   void threadForceTimerCompletionWithError();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a thread that executes long term qcalls. That is, execution of
// thread qcalls is expected to take a long amount of time.

class TwoThreadLongThread : public Ris::Threads::BaseQCallThread
{
public:
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Base class for a two thread. It has a short term thread and a long term
// thread.

class BaseTwoThread
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // The two threads. 
   TwoThreadShortThread*  mShortThread;
   TwoThreadLongThread*   mLongThread;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // If this is nonzero, then it is used to throw an
   // exception when a timer completion abort is caused.
   int mTimerCompletionAbortException;

   // If this is nonzero, then it is used to throw an
   // exception when a timer completion timeout happens.
   int mTimerCompletionTimeoutException;

   // If this is nonzero, then it is used to throw an
   // exception when an error is returned in a notification.
   int mTimerCompletionErrorException;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Notification latch
   Ris::Logic::AndOrLatch mNotifyLatch;

   // This is used by the above waits.
   // If true  then waiting for any notification.
   // If false then waiting for all notifications.
   bool mWaitingForNotifyAny;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // If true then the shutdown order is long,short. If false then the
   // shutdown order is short,long.
   bool mShutdownLongShort;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Infastucture.

   // Constructor
   BaseTwoThread();
   virtual ~BaseTwoThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Launch, shutdown the two threads.
   void launchThreads(); 
   void shutdownThreads();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. These are overloaded by inheritors. They execute in the context
   // of the short thread. They are called by the associated short term thread
   // functions.

   virtual void threadInitFunction(){}
   virtual void threadExitFunction(){}
   virtual void threadExceptionFunction(char* aStr){}
   virtual void executeOnTimer(int aTimerCount){}

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Reset the notification latch.
   void resetNotify();

   // Wait for the timer.
   // This is called from the long thread.
   // This waits for a timer completion.
   // If the wait is aborted then it can throw an exception.
   // It returns a TimerCompletion code.
   int waitForTimer(int aTimeout = -1);

   // Wait for a notification from short thread.
   // This is called from the long thread.
   // This waits for a timer completion.
   // If the wait times out  then it can throw an exception.
   // If the wait is aborted then it can throw an exception.
   // It returns a TimerCompletion code.
   int waitForNotify(int aTimeout = -1);

   // Wait for a notification from short thread.
   // This is called from the long thread.
   // One notification in the list.
   void waitForNotify(int aTimeout, int aIndex);

   // Wait for a notification from short thread.
   // This is called from the long thread.
   // Any notifications in the list.
   void waitForNotifyAny(int aTimeout, int aNumArgs, ...);

   // Wait for a notification from short thread.
   // This is called from the long thread.
   // All notifications in the list.
   void waitForNotifyAll(int aTimeout, int aNumArgs, ...);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Notify the long thread with a latch index.
   // This is called from the short thread.
   // This forces a timer completion, waking up the above waits.
   void notify(int aIndex);

   // Abort the long thread from waiting for notification from short thread.
   // This is called from the short thread.
   // This aborts the wait for a timer completion.
   void abortWaitForNotify();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Send notification qcall.
   // This is executed by the short thread to call notify(aIndex), which then
   // notifies the long term thread.

   Ris::Threads::QCall3<int,int,void*> mSendNotifyQCall;
   void executeSendNotify(int aIndex,int aStatus,void* aData);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a notification callback. It invokes a two thread qcall that executes
// in the context of the short term thread to notify the long term thread.
// A two thread master can use this by passing it to a slave thread that it
// sends a command to. The slave thread can then use it to notify the master
// thread when the command is completed.

class TwoThreadNotify
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // The two thread for which the notifications is intended.
   BaseTwoThread* mTwoThread;

   // Notification index.
   int mIndex;

   // Notification status. Zero means no error.
   int mStatus;

   // Generic data pointer that can be used to return some data with the 
   // notification.
   void* mData;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Intrastructure.

   // Constructor.
   TwoThreadNotify();
   TwoThreadNotify(BaseTwoThread* aTwoThread,int aIndex);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Clear the status and data to zero.
   void clear();

   // Set the status to a nonzero value. This will cause a notification 
   // error exception on the thread waiting for the notification.
   void setError();

   // Sets the data to a nonzero value. This will pass the data to  
   // the thread waiting for the notification.
   void setData(void* aData);

   // Send notify. This invokes the two thread qcall to execute in the 
   // context of the short term thread to notify the long term thread.
   void notify();

};
//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace


