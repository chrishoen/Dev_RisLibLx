/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "my_functions.h"

#include <stdarg.h>

#include "risThreadsNotify.h"

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

Notify::Notify()
{
   reset();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Reset the bit mask.

void Notify::reset()
{
   mLock = true;
   for (int i = 0; i < cMaxBits; i++)
   {
      mMask[i]   = false;
      mLatch[i]  = false;
   }
   mErrorCode = 0;
   mLabel[0] = 0;
   mException[0] = 0;
   mAnyFlag = false;
   mAbortFlag = false;
   mTimeoutFlag = false;
   mErrorFlag = 0;
   mEventSem.reset();
}

void Notify::clearFlags()
{
   mAbortFlag = false;
   mTimeoutFlag = false;
   mErrorFlag = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Clear all of the mask and latch bits and set a single mask bit.

void Notify::setMaskOne(int aBitNum)
{
   // Test for exception conditions.
   testException();

   // Reset all variables and reset the event semaphore.
   reset();

   // Set the mask bit.
   mMask[aBitNum] = true;

   // Set the any flag for the OR trap condition.
   mAnyFlag = true;

   // Enable notifications.
   mLock = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Clear all of the mask and latch bits and set a single mask bit.

void Notify::setMaskOne(const char* aLabel,int aBitNum)
{
   // Test for exception conditions.
   testException();

   // Reset all variables and reset the event semaphore.
   reset();

   // Set the label.
   my_strncpy(mLabel, aLabel, cMaxStringSize);

   // Set the mask bit.
   mMask[aBitNum] = true;

   // Set the any flag for the OR trap condition.
   mAnyFlag = true;

   // Enable notifications.
   mLock = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Clear all of the mask and latch bits and set a variable list of mask bits.
// Set the trap condition for OR.

void Notify::setMaskAny(int aNumArgs, ...)
{
   // Test for exception conditions.
   testException();

   // Reset all variables and reset the event semaphore.
   reset();

   // Set the mask bit from variable arguments.
   va_list valist;
   va_start(valist, aNumArgs);
   for (int i = 0; i < aNumArgs; i++)
   {
      int tBitNum = va_arg(valist, int);
      mMask[tBitNum] = true;
   }
   va_end(valist);

   // Set the any flag for the OR trap condition.
   mAnyFlag = true;

   // Enable notifications.
   mLock = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Clear all of the mask and latch bits and set a variable list of mask bits.
// Set the trap condition for OR.

void Notify::setMaskAny(const char* aLabel,int aNumArgs, ...)
{
   // Test for exception conditions.
   testException();

   // Reset all variables and reset the event semaphore.
   reset();

   // Set the label.
   my_strncpy(mLabel, aLabel, cMaxStringSize);

   // Set the mask bit from variable arguments.
   va_list valist;
   va_start(valist, aNumArgs);
   for (int i = 0; i < aNumArgs; i++)
   {
      int tBitNum = va_arg(valist, int);
      mMask[tBitNum] = true;
   }
   va_end(valist);

   // Set the any flag for the OR trap condition.
   mAnyFlag = true;

   // Enable notifications.
   mLock = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Clear all of the mask and latch bits and set a variable list of mask bits.
// Set the trap condition for AND.

void Notify::setMaskAll(int aNumArgs, ...)
{
   // Test for exception conditions.
   testException();

   // Reset all variables and reset the event semaphore.
   reset();

   // Set the mask bit from variable arguments.
   va_list valist;
   va_start(valist, aNumArgs);
   for (int i = 0; i < aNumArgs; i++)
   {
      int tBitNum = va_arg(valist, int);
      mMask[tBitNum] = true;
   }
   va_end(valist);

   // Set the any flag for the AND trap condition.
   mAnyFlag = false;

   // Enable notifications.
   mLock = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Clear all of the mask and latch bits and set a variable list of mask bits.
// Set the trap condition for AND.

void Notify::setMaskAll(const char* aLabel,int aNumArgs, ...)
{
   // Test for exception conditions.
   testException();

   // Reset all variables and reset the event semaphore.
   reset();

   // Set the label.
   my_strncpy(mLabel, aLabel, cMaxStringSize);

   // Set the mask bit from variable arguments.
   va_list valist;
   va_start(valist, aNumArgs);
   for (int i = 0; i < aNumArgs; i++)
   {
      int tBitNum = va_arg(valist, int);
      mMask[tBitNum] = true;
   }
   va_end(valist);

   // Set the any flag for the AND trap condition.
   mAnyFlag = false;

   // Enable notifications.
   mLock = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Set a bit in the bit latch and conditionally signal the event
// semaphore.

void Notify::notify(int aBitNum)
{
   // Guard.
   if (mLock) return;

   // Set the latch bit.
   mLatch[aBitNum] = true;

   // Test the masked latch bits for OR and AND trap conditions.
   bool tAnyFound = false;
   bool tAllFound = true;
   for (int i = 0; i < cMaxBits; i++)
   {
      if (mMask[i])
      {
         if (mLatch[i]) tAnyFound = true;
         else           tAllFound = false;
      }
   }
   if (!tAnyFound) tAllFound = false;

   // Test if the OR trap condition is true and any latched bits were found
   // or if the AND trap condition is true and all latched birs were found.
   if ((mAnyFlag && tAnyFound) || (!mAnyFlag && tAllFound))
   {
      // Signal the event.
      mEventSem.put();
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Set a bit in the bit latch and conditionally signal the event
// semaphore. Also set the error code.

void Notify::notifyError(int aBitNum, int aError)
{
   mErrorCode = aError;
   mErrorFlag = true;
   notify(aBitNum);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Set the abort bit and signal the event semaphore.

void Notify::abort()
{
   // Set the abort flag.
   mAbortFlag = true;

   // Signal the event.
   mEventSem.put();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Test for an exception condition. If an abort, timeout, or error
// has occurred then throw the corresponding exception.

void Notify::testException()
{
   // Test for an abort condition.
   if (mAbortFlag)
   {
      clearFlags();
      mExceptionCode = cAbortException;
      sprintf(mException, "aborted %s %d", mLabel, mErrorCode);
      throw cAbortException;
   }

   // Test for a timeout condition.
   if (mTimeoutFlag)
   {
      clearFlags();
      mExceptionCode = cTimeoutException;
      sprintf(mException, "timeout %s",mLabel);
      throw cTimeoutException;
   }

   // Test for an error condition.
   if (mErrorFlag)
   {
      clearFlags();
      mExceptionCode = cErrorException;
      sprintf(mException, "error %s %d", mLabel, mErrorCode);
      throw cErrorException;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Wait for a bit to be set. Test for exception conditions.

void Notify::wait(int aTimeout)
{
   // Test for exception conditions.
   testException();

   // Wait for the event. Set the timeout flag if the wait times out.
   if (!mEventSem.get(aTimeout))
   {
      // A timeout occurred.
      mTimeoutFlag = true;
   }

   // Test for exception conditions.
   testException();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Wait for a specified time. Ignore any bit notifications except an abort.
// Test for exception conditions.

void Notify::waitForTimer(int aTimeout)
{
   // Test for exception conditions.
   testException();

   // Reset all variables.
   reset();

   // Set the label.
   my_strncpy(mLabel, "waitForTimer", cMaxStringSize);

   // Wait for the event. Only an abort should signal the event. 
   mEventSem.get(aTimeout);

   // Test for exception conditions.
   testException();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

NotifyWrapper::NotifyWrapper()
{
   mNotify = 0;
   mBitNum = 0;
}

// Constructor.
NotifyWrapper::NotifyWrapper(Notify* aNotify, int aBitNum)
{
   mNotify = aNotify;
   mBitNum = aBitNum;
}

// Set.
void NotifyWrapper::set(Notify* aNotify, int aBitNum)
{
   mNotify = aNotify;
   mBitNum = aBitNum;
}

// Set.
void NotifyWrapper::set(Notify* aNotify)
{
   mNotify = aNotify;
}

// Set.
void NotifyWrapper::set(int aBitNum)
{
   mBitNum = aBitNum;
}

void NotifyWrapper::notify()
{
   if (mNotify) mNotify->notify(mBitNum);
}

void NotifyWrapper::notifyError(int aErrorCode)
{
   if (mNotify) mNotify->notifyError(mBitNum,aErrorCode);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

