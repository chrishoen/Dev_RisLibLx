/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <new>

#include "my_functions.h"
#include "my_defs.h"
#include "risMemoryPtr.h"
#include "risLMPacketQueue.h"

using namespace std;

namespace Ris
{
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor, initialize members for an empty stack of size zero 

int LMPacketQueueState::getMemorySize()
{
   return my_round_upto16(sizeof(LMPacketQueueState));
}

LMPacketQueueState::LMPacketQueueState()
{
   // All null
   mNumElements=0;
   mQueueNumElements=0;
   mListNumElements=0;
   mElementSize=0;
}

void LMPacketQueueState::initialize(int aNumElements,int aElementSize,bool aConstructorFlag)
{
   // Do not initialize, if already initialized.
   if (!aConstructorFlag) return;

   // Allocate for one extra dummy node.
   mNumElements       = aNumElements + 1;
   // Allocate for one extra dummy node.
   mQueueNumElements  = aNumElements + 1;
   // Allocate for one extra dummy node.
   mListNumElements   = aNumElements + 1;
   // Store.
   mElementSize       = aElementSize;
}

//***************************************************************************
//***************************************************************************
//***************************************************************************
// This local class calculates and stores the memory sizes needed by the class.

class LMPacketQueue::MemorySize
{
public:
   // Members.
   int mStateSize;
   int mQueueArraySize;
   int mListArraySize;
   int mElementArraySize;
   int mMemorySize;

   // Calculate and store memory sizes.
// MemorySize::MemorySize(int aNumElements, int aElementSize)
   MemorySize(int aNumElements, int aElementSize)
   {
      mStateSize         = LMPacketQueueState::getMemorySize();
      mQueueArraySize    = my_round_upto16(cNewArrayExtraMemory + (aNumElements + 1)*sizeof(int));
      mListArraySize     = my_round_upto16(cNewArrayExtraMemory + (aNumElements + 1)*sizeof(int));
      mElementArraySize  = my_round_upto16(cNewArrayExtraMemory + (aNumElements + 1)*aElementSize);
      mMemorySize = mStateSize + mQueueArraySize + mListArraySize + mElementArraySize;
   }
};

//***************************************************************************
//***************************************************************************
//***************************************************************************
// This returns the number of bytes that an instance of this class
// will need to be allocated for it.

int LMPacketQueue::getMemorySize(int aNumElements,int aElementSize)
{
   MemorySize tMemorySize(aNumElements,aElementSize);
   return tMemorySize.mMemorySize;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

LMPacketQueue::LMPacketQueue()
{
   // All null.
   mX = 0;
   mOwnMemoryFlag = false;
   mMemory = 0;

   // All null
   mElement = 0;
   mQueueNext = 0;
   mListNext = 0;
}

LMPacketQueue::~LMPacketQueue()
{
   finalize();
}

//***************************************************************************
//***************************************************************************
//***************************************************************************
// Initialize

void LMPacketQueue::initialize(int aNumElements,int aElementSize)
{
   initialize(aNumElements,aElementSize,true,0);
}

void LMPacketQueue::initialize(int aNumElements,int aElementSize,bool aConstructorFlag, void* aMemory)
{
   //------------------------------------------------------------------------
   //------------------------------------------------------------------------
   //------------------------------------------------------------------------
   // Initialize memory.

   // Deallocate memory, if any exists.
   finalize();

   // If the instance of this class is not to reside in external memory
   // then allocate memory for it on the system heap.
   if (aMemory == 0)
   {
      mMemory = malloc(LMPacketQueue::getMemorySize(aNumElements,aElementSize));
      mOwnMemoryFlag = true;
   }
   // If the instance of this class is to reside in external memory
   // then use the memory pointer that was passed in.
   else
   {
      mMemory = aMemory;
      mOwnMemoryFlag = false;
   }

   // Calculate memory sizes.
   MemorySize tMemorySize(aNumElements,aElementSize);

   // Calculate memory addresses.
   MemoryPtr tMemoryPtr(mMemory);

   char* tStateMemory        = tMemoryPtr.cfetch_add(tMemorySize.mStateSize);
   char* tQueueArrayMemory   = tMemoryPtr.cfetch_add(tMemorySize.mQueueArraySize);
   char* tListArrayMemory    = tMemoryPtr.cfetch_add(tMemorySize.mListArraySize);
   char* tElementArrayMemory = tMemoryPtr.cfetch_add(tMemorySize.mElementArraySize);

   // Construct the state.
   if (aConstructorFlag)
   {
      // Call the constructor.
      mX = new(tStateMemory)LMPacketQueueState;
   }
   else
   {
      // The constructor has already been called.
      mX = (LMPacketQueueState*)tStateMemory;
   }
   // Initialize the state.
   mX->initialize(aNumElements,aElementSize,aConstructorFlag);

   // Construct the arrays.
   if (aConstructorFlag)
   {
      // Call the constructor.
      mQueueNext = new(tQueueArrayMemory)int[mX->mQueueNumElements];

      // Call the constructor.
      mListNext = new(tListArrayMemory)int[mX->mListNumElements];
   }
   else
   {
      // The constructor has already been called.
      mQueueNext = (int*)tQueueArrayMemory;

      // The constructor has already been called.
      mListNext = (int*)tListArrayMemory;
   }

   // There is no constructor for this array.
   mElement = tElementArrayMemory;

   //------------------------------------------------------------------------
   //------------------------------------------------------------------------
   //------------------------------------------------------------------------
   // Initialize variables.

   // Initialize queue and linked list, if not already been initialized.
   if (aConstructorFlag)
   {
      // Initialize linked list array. Each node next node is the one after it.
      for (int i = 0; i < mX->mListNumElements - 1; i++)
      {
         mListNext[i] = i + 1;
      }
      // The last node has no next node.
      mListNext[mX->mListNumElements - 1] = cInvalid;

      // List head points to the first node.
      mX->mListHead = 0;
      // List size is initially a full stack.
      mX->mListSize = mX->mListNumElements;

      // Initialize queue array. Each node has no next node.
      for (int i = 0; i < mX->mListNumElements; i++)
      {
         mQueueNext[i] = cInvalid;
      }

      // Pop the dummy node.
      int tDummyNode;
      listPop(&tDummyNode);

      // Initialize queue head and tail.
      mX->mQueueHead = tDummyNode;
      mX->mQueueTail = mX->mQueueHead;
   }
}

//***************************************************************************
//***************************************************************************
//***************************************************************************

void LMPacketQueue::finalize()
{
   if (mOwnMemoryFlag)
   {
      if (mMemory)
      {
         free(mMemory);
      }
   }
   mMemory = 0;
   mOwnMemoryFlag = false;
}

//***************************************************************************
//***************************************************************************
//***************************************************************************
// Size

int LMPacketQueue::size()
{
   return mX->mListNumElements - mX->mListSize;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return a pointer to a packet, based on its packet index.

void* LMPacketQueue::elementAt(int aIndex)
{
   return (void*)((char*)mElement + mX->mElementSize*aIndex);
}

//***************************************************************************
//***************************************************************************
//***************************************************************************
// This attempts to write a value to the queue. If the queue is not full
// then it succeeds. It attempts to pop a node from the free list. If
// the free list is empty then the queue is full and it exits. The value
// is to be written is stored in the new node. The new node is then attached
// to the queue tail node and the tail index is updated.

void* LMPacketQueue::startWrite(int* aNodeIndex)
{
   // Try to allocate a node from the free list.
   // Exit if it is empty.

   mListMutex.lock();

   int tNodeIndex;
   if (!listPop(&tNodeIndex))
   {
      mListMutex.unlock();
      return 0;
   }

   mListMutex.unlock();

   // Initialize the node.
   mQueueNext[tNodeIndex] = cInvalid;

   // Return a pointer to the node element.
   *aNodeIndex = tNodeIndex;
   return elementAt(tNodeIndex);
}

void LMPacketQueue::finishWrite(int aNodeIndex)
{
   mTailMutex.lock();

   // Attach the node to the queue tail.
   mQueueNext[mX->mQueueTail] = aNodeIndex;
   mX->mQueueTail = aNodeIndex;

   mTailMutex.unlock();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This attempts to read a value from the queue. If the queue is not empty
// then it succeeds. The next node in the queue to be read is the one 
// immedialtely after the head node. It extracts the read value from the read
// node, pushes the previous head node back onto the free list and updates the
// head index.

void* LMPacketQueue::startRead(int* aNodeIndex)
{
   // Store int temps.
   void* tElementPtr = 0;
   int tNode    = mX->mQueueHead;
   int tNewHead = mQueueNext[tNode];

   // Exit if queue is empty.
   if (tNewHead == cInvalid) return 0;

   // Store new head.
   tElementPtr = elementAt(tNewHead);
   mX->mQueueHead = tNewHead;

   // Return new head.
   *aNodeIndex = tNewHead;
   *aNodeIndex = tNode;

   // Done.
   return tElementPtr;
}

void LMPacketQueue::finishRead(int aNodeIndex)
{
   mListMutex.lock();
   listPush(aNodeIndex);
   mListMutex.unlock();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This detaches the head node.

bool LMPacketQueue::listPop(int* aNode)
{
   // Store the head node in a temp.
   // This is the node that will be detached.
   int tHead = mX->mListHead;

   // Exit if the list is empty.
   if (tHead == cInvalid) return false;

   // Set the head node to be the node that is after the head node.
   mX->mListHead = mListNext[tHead];

   // Return the detached original head node.
   *aNode = tHead;

   // Done.
   mX->mListSize--;
   return true;
}

//***************************************************************************
//***************************************************************************
//***************************************************************************
// Insert a node into the list before the list head node.

bool LMPacketQueue::listPush(int aNode)
{
   // Store the head node in a temp.
   int tHead = mX->mListHead;

   // Attach the head node to the pushed node.
   mListNext[aNode] = tHead;

   // The pushed node is the new head node.
   mX->mListHead = aNode;

   // Done.
   mX->mListSize++;
   return true;
}

//***************************************************************************
//***************************************************************************
//***************************************************************************
// Insert a node into the list before the list head node.

void LMPacketQueue::lockList()
{
   mListMutex.lock();
}

void LMPacketQueue::unlockList()
{
   mListMutex.unlock();
}

void LMPacketQueue::lockTail()
{
   mTailMutex.lock();
}

void LMPacketQueue::unlockTail()
{
   mTailMutex.unlock();
}

}//namespace