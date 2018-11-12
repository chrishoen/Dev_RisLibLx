#pragma once

/*==============================================================================

Value queue that uses critical sections as locks.
Single Reader Multiple Writer Value Queue class template.
It is multiple writer single reader thread safe.
It locks with critcal sections for mutual exclusion, blocking.

==============================================================================*/
//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risLCPointerQueue.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

LCPointerQueue::LCPointerQueue()
{
   mElement = 0;
   mCriticalSection = createCriticalSection();
   resetVariables();
}

void LCPointerQueue::resetVariables()
{
   mValidFlag = false;
   mNumElements = 0;
   mPutIndex = 0;
   mGetIndex = 0;
   mWritePass = 0;
   mWriteFail = 0;
   mReadPass = 0;
   mReadFail = 0;
}

   LCPointerQueue::~LCPointerQueue()
{
   finalize();
   destroyCriticalSection(mCriticalSection);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Allocate memory for the queue and initialize variables.

void LCPointerQueue::initialize(int aSize)
{
   // Deallocate memory, if any exists.
   finalize();

   // Initialize.
   resetVariables();

   // Allocate for one extra dummy element.
   mNumElements = aSize + 1;

   // Allocate memory for the array.
   mElement = new void*[mNumElements];

   // The queue is now valid.
   mValidFlag = true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Deallocate the memory.

void LCPointerQueue::finalize()
{
   mValidFlag = false;
   if (mElement == 0) return;
   delete mElement;
   mElement = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return the current size of the queue.
//
// This is the current size of the queue. It is the difference between the 
// mPutIndex and the mGetIndex.

int LCPointerQueue::size()
{
   int tSize = mPutIndex - mGetIndex;
   if (tSize < 0) tSize = mNumElements + tSize;
   return tSize;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show the metrics.

void LCPointerQueue::show(char* aLabel)
{
   printf("LCPointerQueue %s WritePass  %d\n", aLabel, mWritePass);
   printf("LCPointerQueue %s WriteFail  %d\n", aLabel, mWriteFail);
   printf("LCPointerQueue %s ReadPass   %d\n",  aLabel, mReadPass);
   printf("LCPointerQueue %s ReadPass   %d\n",  aLabel, mReadFail);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Try to write a pointer to the queue. Return true if successful. 
// Return false if the queue is full.
//
// This attempts to write a value to the queue. If the queue is not full
// then it succeeds.
// 
// This tests if put operations are allowed. Puts are allowed if the 
// current size is less than or equal to Allocate - 2. If the size is equal
// to Allocate - 2 then the next put operation would put the size to
// mNumElements - 1, which is the max number of elements. This is the same
// as "is not full".
// 
// This puts an element to the queue and advances the put index. It does a 
// copy from a source element into the queue array element at the put index.
// It uses a temp index variable so that writing to the index is atomic.

bool LCPointerQueue::tryWrite (void* aElement)
{
   // Guard.
   if (!mValidFlag) return false;

   // Lock.
   enterCriticalSection(mCriticalSection);

   // Exit if the queue is full.
   int tSize = mPutIndex - mGetIndex;
   if (tSize < 0) tSize = mNumElements + tSize;
   if (tSize > mNumElements - 2)
   {
      // Unlock.
      leaveCriticalSection(mCriticalSection);
      mWriteFail++;
      return false;
   }

   // Local put index.
   int tPutIndex = mPutIndex;
   // Copy the source element into the element at the queue put index
   mElement[tPutIndex] = aElement;
   // Advance the put index.
   if(++tPutIndex == mNumElements) tPutIndex = 0;
   mPutIndex = tPutIndex;

   // Unlock.
   leaveCriticalSection(mCriticalSection);

   // Done.
   mWritePass++;
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Try to read a pointer from the queue. Return the pointer if successful.
// Return zero if the queue is empty.
//
// This attempts to read a value to the queue. If the queue is not empty
// then it succeeds.
//
// This gets an element from the queue and advances the get index. It does a 
// copy from the queue array element at the get index into a destination
// element. It uses a temp index variable so that writing to the index is
// atomic.
  
void* LCPointerQueue::tryRead()
{
   // Guard.
   if (!mValidFlag) return 0;

   // Exit if the queue is empty.
   int tSize = mPutIndex - mGetIndex;
   if (tSize < 0) tSize = mNumElements + tSize;
   if (tSize == 0)
   {
      mReadFail++;
      return 0;
   }

   // Local index.
   int tGetIndex = mGetIndex;
   // Copy the queue array element at the get index.
   void* tValue = mElement[tGetIndex];
   // Advance the get index.
   if(++tGetIndex == mNumElements) tGetIndex = 0;
   mGetIndex = tGetIndex;

   // Done.
   mReadPass++;
   return tValue;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

