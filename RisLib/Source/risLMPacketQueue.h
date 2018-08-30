#ifndef _RISLMPACKETQUEUE_H_
#define _RISLMPACKETQUEUE_H_
/*==============================================================================

Locked Mutex Packet Queue.

It is multiple writer single reader thread safe.
It lockes with semaphores, blocking.
It is shared memory safe.
It is zero copy.

This implements a queue of fixed size packets, where the queue provides the
memory allocation for the packets (memory for the packets is contained
within the queue). The queue is thread safe. It uses mutexes to guard 
against concurrency contentions.

It is thread safe for separate multiple writer and single reader threads.

It implements the Michael and Scott algorithm for blocking queues. It 
uses mutex protection. It maintains storage for the packets by implementing a
free list that also uses mutex protection. 

=============================================================================*/
//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risThreadsSynch.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// State variables for the object. These are located in a separate class
// so that they can be located in external memory.

class LMPacketQueueState
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // This returns the number of bytes that an instance of this class
   // will need to be allocated for it.

   static int getMemorySize();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Number of packets allocated.
   int mNumElements;
   int mQueueNumElements;
   int mListNumElements;
   int mElementSize;

   // Queue variables.
   int     mQueueHead;
   int     mQueueTail;

   // Linked list variables.
   int     mListHead;
   int     mListSize;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   LMPacketQueueState();

   // Initialize.
   void initialize(int aNumElements,int aElementSize,bool aConstructorFlag);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Lock free packet queue class.

class LMPacketQueue
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // This returns the number of bytes that an instance of this class
   // will need to be allocated for it.

   static int getMemorySize(int aNumElements,int aElementSize);

   class MemorySize;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members

   // If this flag is false then the memory for this object was created
   // externally. If it is true then the memory was allocated at 
   // initialization and must be freed at finalization.
   bool mOwnMemoryFlag;

   // Pointer to memory for which the queue resides. This is either created
   // externally and passed as an initialization parameter or it is created
   // on the system heap at initialization.
   void* mMemory;

   // State variables for the queue. These are located in a separate class
   // so that they can be located in externale memory.
   LMPacketQueueState* mX;

   // Array of bytes, storage for the packets.
   // Size is NumElements + 1.
   // Index range is 0..NumElements.
   void* mElement;

   // Queue array, contains the node index of the queue next node.
   // Size is NumElements + 1.
   // Index range is 0..NumElements.
   int* mQueueNext;

   // Free List array, contains the node index of the free list next node.
   // Size is NumElements + 1.
   // Index range is 0..NumElements.
   int* mListNext;

   // Marks an invalid node.
   static const int  cInvalid = 0x80000000;

   // Mutex.
   Ris::Threads::MutexSemaphore mTailMutex;
   Ris::Threads::MutexSemaphore mListMutex;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialization Methods.
   //
   // Initialization parameters:
   // Number of elements to create. There is one extra dummy element, so that
   // memory for aNumElements+1 is allocated.
   //    int aNumElements
   //
   // Element byte size.
   //    int aElementSize
   //
   // If true then constructors are called for all internal objects created
   // for the object. If false then the constructors are not called.
   // This is false if the object resides in shared memory and has
   // already been created. This is true otherwise.
   //    bool aConstructorFlag
   //
   // Pointer to external memory allocated for the entire object.
   // If this is null then system heap memory is allocated for the entire
   // block pool.
   //    void* aMemory

   // Constructor
   LMPacketQueue();
  ~LMPacketQueue();

   // Allocate memory for the queue and free list arrays and initialize the
   // queue variables. 
   void initialize(int aNumElements,int aElementSize);
   void initialize(int aNumElements,int aElementSize,bool aConstructorFlag, void* aMemory);

   // Deallocate memory.
   void finalize();

   // Queue size
   int size();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Queue and Free List Methods. These write or read packets from the queue
   // and pop or push node indices from the free list.

   // These are used to enqueue a packet. StartWrite pops a packet from the 
   // free list and returns a pointer to it. If the queue is full then it 
   // returns null. After writing to the packet, FinishWrite is called to 
   // enqueue it at the queue tail. The node index is passed between the two
   // methods.

   void* startWrite  (int* aNodeIndex);
   void  finishWrite (int  aNodeIndex);

   // These are used to dequeue a packet. StartRead dequeues a packet from the 
   // queue head and returns a pointer to it. After reading from the packet,
   // FinishRead is called to push it onto the free list. The node index is 
   // passed between the two methods.

   void* startRead   (int* aNodeIndex);
   void  finishRead  (int  aNodeIndex);

   // These are called by the above write and read methods. 
   bool  listPop     (int* aNode);
   bool  listPush    (int  aNode);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Mutex Methods.

   void lockList();
   void unlockList();
   void lockTail();
   void unlockTail();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Helpers:
private:
   // Return a pointer to a packet, based on its packet index.
   void* elementAt(int aIndex);
};

//******************************************************************************

}//namespace
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Example

#if 0
   //---------------------------------------------------------------------------
   // Management

   // includes
   #include <new>
   #include "ccLMPacketQueue.h"

   // Declare packet queue
   CC::LMPacketQueue mPacketQueue;
   // Initialize packet queue
   mPacketQueue.initialize(100,sizeof(Class1A));

   //---------------------------------------------------------------------------
   // Enqueue a packet

   // Example counter
   int tWriteCount;
   // This is passed between StartWrite and FinishWrite
   int tWriteIndex;

   // Try to start a write to allocate a packet
   void* tElement = mPacketQueue.startWrite(&tWriteIndex);
   // If the queue is not full
   if (tElement)
   {
      // Create a new object with placement new on the allocated packet.
      // Placement new must be used with any classes that use vtables.
      Class1A* tObject = new(tElement) Class1A;
      // Access the new object
      tObject->mCode1 = tWriteCount;
      // Finish the write
      mPacketQueue.finishWrite(tWriteIndex);
   }

   //---------------------------------------------------------------------------
   // Dequeue a packet 

   // Example counter
   int tReadCount;
   // This is passed between StartRead and FinishRead
   int tReadIndex;

   // Try to start a packet read, returns a pointer to an object
   Class1A* tObject = (Class1A*)mPacketQueue.startRead(&tReadIndex);
   // If the queue is not empty
   if (tObject)
   {
      // Access the object
      tReadCount = tObject->mCode1;
      // Finish the read
      mPacketQueue.finishRead(tReadIndex);
   }
#endif

