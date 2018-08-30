#pragma once
/*==============================================================================

thread queues

class Queue_OneToOne<class Element,int Allocate>
class Queue_OneToOneWaitable<class Element,int Allocate>
class Queue_ManyToOneWaitable<class Element,int Allocate>
class Queue_ManyToOneWaitableHiLo<class Element,int Allocate>

==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risPortableTypes.h"
#include "prnPrint.h"

#include "risContainers.h"
#include "risThreadsThreads.h"

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a template for a queue of items. It can be used by a single
// thread to send items to another single thread (single thread to single
// threads).

template <class Element,int Allocate>
class  Queue_OneToOne : public Containers::Queue<Element,Allocate>
{
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a template for a queue of items. It can be used by a single
// thread to send items to another single thread (single thread to single
// threads).It also provides a counting semaphore, at which the single
// consumer thread can wait at.

template <class Element,int Allocate>
class  Queue_OneToOneWaitable : public Containers::Queue<Element,Allocate>
{
public:
   typedef Containers::Queue<Element,Allocate> BaseClass;

   //***************************************************************************
   bool put(const Element & V)
   {
     bool success = false;
     if (BaseClass::isNotFull())
     {
        success = BaseClass::put(V);
        mCountingSem.put();
     }
     else
     {
        success = false;
     }

     return success;
   }

   //***************************************************************************
   int waitFor (int timeout = -1)
   {
      return mCountingSem.get(timeout);
   }

   //***************************************************************************
   CountingSemaphore mCountingSem;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

class  WaitableLimit
{
public:
   WaitableLimit(int aLimit) :
      mSem(aLimit)
   {}

   void put()
   {
      mSem.put();
   }
   void get()
   {
      mSem.get();
   }

   //***************************************************************************
   CountingSemaphore mSem;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a template for a queue of items that is protected on the
// input side by a mutex semaphore. It can be used by a set of threads
// to send items to a single thread (multiple threads to a single thread).
// It also provides a counting semaphore, at which the single consumer
// thread can wait at.

template <class Element,int Allocate>
class  Queue_ManyToOneWaitable : public Containers::Queue<Element,Allocate>
{
public:
   typedef Containers::Queue<Element,Allocate> BaseClass;

   Queue_ManyToOneWaitable() : mLimit(Allocate-10) {}

   //***************************************************************************
   bool put(const Element & V)
   {
      bool tRet = false;
      if (BaseClass::isNotFull())
      {
         tRet = BaseClass::put(V);
         mCountingSem.put();
      }
      else
      {
         tRet = false;
      }
      return tRet;
   }

   //***************************************************************************
   Element& get()
   {
      return BaseClass::get();
   }

   //***************************************************************************
   bool waitFor (int timeout = -1)
   {
      return mCountingSem.get(timeout);
   }

   //***************************************************************************
   MutexSemaphore    mMutexSem;
   CountingSemaphore mCountingSem;
   WaitableLimit     mLimit;

   void lock()       {mMutexSem.get();}
   void unlock()     {mMutexSem.put();}
   void putLimit()   {mLimit.put();}
   void getLimit()   {mLimit.get();}

};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace
