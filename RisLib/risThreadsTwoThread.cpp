/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <stdarg.h>

#include "prnPrint.h"

#include "risThreadsTwoThread.h"

using namespace std;

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
// Constructor.

TwoThreadShortThread::TwoThreadShortThread()
{
   mTimerPeriod = 0;
   mTimerCompletionDownCounter = 0;
   mTimerCompletionCode = 0;
}

TwoThreadShortThread::~TwoThreadShortThread()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// BaseClass overload.

void TwoThreadShortThread::threadInitFunction() 
{
   // Call the call pointer.
   if (mThreadInitCallPointer) mThreadInitCallPointer();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// BaseClass overload.

void TwoThreadShortThread::threadExitFunction() 
{
   // Call the call pointer.
   if (mThreadExitCallPointer) mThreadExitCallPointer();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// BaseClass overload.

void TwoThreadShortThread::threadExceptionFunction(char* aStr)
{
   // Call the call pointer.
   if (mThreadExceptionCallPointer) mThreadExceptionCallPointer(aStr);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// BaseClass overload.

void TwoThreadShortThread::executeOnTimer(int aTimerCount)
{
   // Post to the timer completion semaphore,
   // If the down counter counts down to zero.
   if (mTimerCompletionDownCounter != 0)
   {
      if (--mTimerCompletionDownCounter == 0)
      {
         mTimerCompletionSem.put();
      }
   }

   // Call the call pointer.
   if (mThreadExecuteOnTimerCallPointer) mThreadExecuteOnTimerCallPointer(aTimerCount);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

int TwoThreadShortThread::threadWaitForTimerCompletion(int aTimerCount) 
{
   // Guard.
   if (aTimerCount==0) return 0;

   // Initialize completion code.
   mTimerCompletionCode = TimerCompletion_Timeout;
   // Reset timer completion.
   mTimerCompletionSem.reset();
   // Set the down counter, -1 means infinite timeout.
   mTimerCompletionDownCounter = aTimerCount != -1 ? aTimerCount : 0;
   
   // Wait for timer completion.
   mTimerCompletionSem.get();

   // Return completion code.
   return mTimerCompletionCode;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TwoThreadShortThread::threadAbortTimerCompletion()
{
   // Set completion code.
   mTimerCompletionCode = TimerCompletion_Aborted;
   // Clear down counter.
   mTimerCompletionDownCounter = 0;

   // Post to timer completion.
   // This wakes up the above wait.
   mTimerCompletionSem.put();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TwoThreadShortThread::threadForceTimerCompletion()
{
   // Set completion code.
   mTimerCompletionCode = TimerCompletion_Forced;
   // Clear down counter.
   mTimerCompletionDownCounter = 0;

   // Post to timer completion.
   // This wakes up the above wait.
   mTimerCompletionSem.put();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TwoThreadShortThread::threadForceTimerCompletionWithError()
{
   // Set completion code.
   mTimerCompletionCode = TimerCompletion_ForcedError;
   // Clear down counter.
   mTimerCompletionDownCounter = 0;

   // Post to timer completion.
   // This wakes up the above wait.
   mTimerCompletionSem.put();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

BaseTwoThread::BaseTwoThread()
{
   // Create thread objects.
   mShortThread = new TwoThreadShortThread;
   mLongThread  = new TwoThreadLongThread;

   mShortThread->setThreadPrintLevel(TS::PrintLevel(0, 3));
   mShortThread->setThreadPriority(Ris::Threads::gPriorities.mShort);
   mLongThread->mTimerPeriod = 0;
   
   mLongThread->setThreadPrintLevel(TS::PrintLevel(0, 3));
   mLongThread->setThreadPriority(Ris::Threads::gPriorities.mLong);
   mLongThread->mTimerPeriod = 0;

   // Default exception codes.
   mTimerCompletionAbortException   = 666;
   mTimerCompletionTimeoutException = 667;   
   mTimerCompletionErrorException   = 668;

   mShutdownLongShort = true;

   // Bind qcall.
   mSendNotifyQCall.bind(mShortThread,this,&BaseTwoThread::executeSendNotify);
}

BaseTwoThread::~BaseTwoThread()
{
   delete mShortThread;
   delete mLongThread;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Launch

void BaseTwoThread::launchThreads()
{
   mLongThread->launchThread();
   mShortThread->launchThread();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Shutdown

void BaseTwoThread::shutdownThreads() 
{
   // Abort any pending waits.
   mLongThread->mQCallAbortFlag = true;
   mShortThread->threadAbortTimerCompletion();
   // Shutdown the threads.
   if (mShutdownLongShort)
   {
      mLongThread->shutdownThread();
      mShortThread->shutdownThread();
   }
   else
   {
      mShortThread->shutdownThread();
      mLongThread->shutdownThread();
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show thread state info.

void BaseTwoThread::showThreadInfo()
{
   mLongThread->showThreadInfo();
   mShortThread->showThreadInfo();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Abort by aborting a timer completion.

void BaseTwoThread::abortWaitForNotify()
{
   mLongThread->mQCallAbortFlag = true;
   mShortThread->threadAbortTimerCompletion();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Wait for timer.
// This is abortable.

int BaseTwoThread::waitForTimer(int aTimeout)
{
   // Test for abort flag.
   if (mLongThread->mQCallAbortFlag)
   {
      // Throw exception for abort.
      if (mTimerCompletionAbortException != 0)
      {
         throw mTimerCompletionAbortException;
      }
   }

   // Exit if zero.
   if (aTimeout==0) return TwoThreadShortThread::TimerCompletion_Timeout;

   // Timer count from timeout.
   int tTimerCount = aTimeout/mShortThread->mTimerPeriod + 1;
   if (aTimeout < 0) tTimerCount = -1;

   // Wait for timer completion.
   int tTimerCompletionCode = mShortThread->threadWaitForTimerCompletion(tTimerCount);

   // Process timer completion.
   switch (tTimerCompletionCode)
   {
      case TwoThreadShortThread::TimerCompletion_Aborted :
         // Throw exception for abort.
         if (mTimerCompletionAbortException != 0)
         {
            throw mTimerCompletionAbortException;
         }
         break;
      case TwoThreadShortThread::TimerCompletion_ForcedError :
         // Throw exception for abort.
         if (mTimerCompletionErrorException != 0)
         {
            throw mTimerCompletionErrorException;
         }
         break;
   }

   // Return timer completion code.
   return tTimerCompletionCode;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseTwoThread::resetNotify()
{
   // Test for abort flag.
   if (mLongThread->mQCallAbortFlag)
   {
      // Throw exception for abort.
      if (mTimerCompletionAbortException != 0)
      {
         throw mTimerCompletionAbortException;
      }
   }

   // Reset latch.
   mNotifyLatch.reset();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Wait for a single notification.
// This is abortable and has a timeout.

int BaseTwoThread::waitForNotify(int aTimeout)
{
   // Test for abort flag.
   if (mLongThread->mQCallAbortFlag)
   {
      // Throw exception for abort.
      if (mTimerCompletionAbortException != 0)
      {
         throw mTimerCompletionAbortException;
      }
   }

   // Timer count from timeout.
   int tTimerCount = aTimeout/mShortThread->mTimerPeriod + 1;
   if (aTimeout < 0) tTimerCount = -1;

   // Wait for timer completion.
   int tTimerCompletionCode = mShortThread->threadWaitForTimerCompletion(tTimerCount);

   // Process timer completion.
   switch (tTimerCompletionCode)
   {
      case TwoThreadShortThread::TimerCompletion_Aborted :
         // Throw exception for abort.
         if (mTimerCompletionAbortException != 0)
         {
            throw mTimerCompletionAbortException;
         }
         break;
      case TwoThreadShortThread::TimerCompletion_Timeout :
         // Throw exception for abort.
         if (mTimerCompletionTimeoutException != 0)
         {
            throw mTimerCompletionTimeoutException;
         }
         break;
      case TwoThreadShortThread::TimerCompletion_ForcedError :
         // Throw exception for abort.
         if (mTimerCompletionErrorException != 0)
         {
            throw mTimerCompletionErrorException;
         }
         break;
   }

   // Return timer completion code.
   return tTimerCompletionCode;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Wait for a single notification.
// This is abortable and has a timeout.
// Uses the notification latch.

void BaseTwoThread::waitForNotify(int aTimeout, int aIndex)
{
   // Set the notification latch mask,
   mNotifyLatch.setMaskBit(aIndex);

   // Set flag to wait for any notification.
   mWaitingForNotifyAny = true;

   // Return if the latch bit is already set.
   if (mNotifyLatch.isAny()) return;

   // Wait for notication.
   waitForNotify(aTimeout);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Wait for a multiple notification.
// This is abortable and has a timeout.
// Uses the notification latch.

void BaseTwoThread::waitForNotifyAny(int aTimeout, int aNumArgs, ...)
{
   // Set the notification latch mask from variable arguments.
   va_list valist;
   va_start(valist,aNumArgs);
   for (int i=0;i<aNumArgs;i++)
   {
      int tIndex = va_arg(valist,int);
      mNotifyLatch.setMaskBit(tIndex);
   }
   va_end(valist);

   // Set flag to wait for any notification.
   mWaitingForNotifyAny = true;

   // Wait for notication.
   waitForNotify(aTimeout);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Wait for a multiple notification.
// This is abortable and has a timeout
// Uses the notification latch.

void BaseTwoThread::waitForNotifyAll(int aTimeout, int aNumArgs, ...)
{
   // Set the notification latch mask from variable arguments.
   va_list valist;
   va_start(valist,aNumArgs);
   for (int i=0;i<aNumArgs;i++)
   {
      int tIndex = va_arg(valist,int);
      mNotifyLatch.setMaskBit(tIndex);
   }
   va_end(valist);

   // Set flag to wait for all notifications.
   mWaitingForNotifyAny = false;

   // Wait for notication.
   waitForNotify(aTimeout);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Multiple notification by forcing a timer completion.
// Uses the notification latch.

void BaseTwoThread::notify(int aIndex)
{
   // Test for notification error.
   if (aIndex == 999)
   {
      mShortThread->threadForceTimerCompletionWithError();
      return;
   }

   // Set notification latch element.
   mNotifyLatch.setLatchBit(aIndex);

   // Temp.
   bool tNotify=false;

   // If waiting for any.
   if (mWaitingForNotifyAny)
   {
      // If any notification in the latch.
      if (mNotifyLatch.isAny()) tNotify=true;
   }
   // If waiting for any.
   else
   {
      // If all notifications are in the latch.
      if (mNotifyLatch.isAll()) tNotify=true;
   }

   // Notify.
   if (tNotify)
   {
      mShortThread->threadForceTimerCompletion();
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This executes in the context of the short term thread to notify the 
// long term thread.

void BaseTwoThread::executeSendNotify(int aIndex,int aStatus,void* aData)
{
   // If the status is okay.
   if (aStatus == 0)
   {
      // Notify the long term thread.
      notify(aIndex);
   }
   // If the status is an error.
   else
   {   
      // Notify the long term thread that there was an error.
      notify(999);
   }
}

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

TwoThreadNotify::TwoThreadNotify()
{
   mTwoThread = 0;
   mIndex = 0;
   mStatus = 0;
   mData = 0;
}

TwoThreadNotify::TwoThreadNotify(BaseTwoThread* aTwoThread,int aIndex)
{
   mTwoThread = aTwoThread;
   mIndex = aIndex;
   mStatus = 0;
   mData = 0;
}

void TwoThreadNotify::clear()
{
   mStatus = 0;
   mData = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This sets the status to a nonzero value. This will cause a notification 
// error exception on the thread waiting for the notification.

void TwoThreadNotify::setError()
{
   mStatus = 999;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This sets the status to a nonzero value. This will pass the data 
// to the thread waiting for the notification.

void TwoThreadNotify::setData(void* aData)
{
   mData = aData;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send notify. This invokes the two thread qcall to execute in the 
// context of the short termm thread to notify the long term thread.

void TwoThreadNotify::notify()
{
   if (mTwoThread==0) return;
   mTwoThread->mSendNotifyQCall(mIndex,mStatus,mData);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace
