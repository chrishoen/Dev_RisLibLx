#pragma once

/*==============================================================================

Pointer queue that uses critical sections as locks.
Single reader multiple writer pointer queue class template.
It is multiple writer single reader thread safe.
It locks with critcal sections for mutual exclusion, blocking.

==============================================================================*/
//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risLCCriticalSection.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Value queue that uses critical sections as locks.
// Single reader multiple writer value queue class template.
// It is multiple writer single reader thread safe.
// It locks with critcal sections for mutual exclusion, blocking.

class LCPointerQueue
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // If true then the queue has been initialized.
   bool mValidFlag;

   // Array of queue values, storage for the values. The values are void*s.
   // NumElements is Size + 1.
   // Index range is 0..Size.
   void** mElement;

   // Number of elements allocated.
   int mNumElements;

   // Queue array indices.
   int mPutIndex;
   int mGetIndex;

   // Metrics.
   int mWritePass;
   int mWriteFail;
   int mReadPass;
   int mReadFail;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Critical section.
   void* mCriticalSection;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   LCPointerQueue();
   ~LCPointerQueue();
   void resetVariables();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Allocate memory for the queue and initialize variables.
   void initialize(int aSize);

   // Deallocate memory.
   void finalize();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Return the current size of the queue.
   int size();

   // Try to write a pointer to the queue. Return true if successful. 
   // Return false if the queue is full.
   bool tryWrite(void* aElement);

   // Try to read a pointer from the queue. Return the pointer if successful.
   // Return zero if the queue is empty.
   void* tryRead();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Show metrics.
   void show(char* aLabel);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

