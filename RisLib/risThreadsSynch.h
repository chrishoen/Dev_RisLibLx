#pragma once
/*==============================================================================
This file contains classes that encapsulate standard rtos multithreading
synchronization constructs. It supplies events, semaphores, and threads.
The purpose of the classes is to wrap the rtos api thread synchronization
calls.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

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
// This encapsulates a binary semaphore.

class BinarySemaphore
{
public:

   BinarySemaphore();            // Creates the semaphore
   virtual ~BinarySemaphore();   // Deletes the semaphore
   void reset();                 // Resets the semaphore

   void put ();                  // Put to the semaphore
   bool get (int timeout= -1);   // Get from the semaphore, block until timeout,
                                 // return true if no timeout

   int mStatusCode;              // User status code

protected:
   class Specific;
   Specific* mSpecific;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This encapsulates a counting semaphore.

class CountingSemaphore
{
public:

   CountingSemaphore();             // Creates the semaphore
   CountingSemaphore(int aInitial); // Creates the semaphore
   virtual ~CountingSemaphore();    // Deletes the semaphore

   void put ();                     // Put to the semaphore
   bool get (int timeout= -1);      // Get from the semaphore, block until timeout,
                                    // return true if no timeout
protected:
   class Specific;
   Specific* mSpecific;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This encapsulates a mutex semaphore.

class MutexSemaphore
{
public:

   MutexSemaphore();             // Creates the mutex semaphore
   virtual ~MutexSemaphore();    // Deletes the mutex semaphore

   void lock   ();               // Lock the mutex
   void unlock ();               // Unlock the mutex

protected:
   class Specific;
   Specific* mSpecific;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This encapsulates a critical section.

class CriticalSection
{
public:

   CriticalSection();
   virtual ~CriticalSection();

   void enter ();
   void leave ();

protected:
   class Specific;
   Specific* mSpecific;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This encapsulates a condition variable

class ConditionVariable
{
public:

   ConditionVariable();
   ~ConditionVariable();

   void acquireLock ();
   void releaseLock ();

   void waitFor ();
   void wakeUp (int aPredicate = 1);

protected:
   class Specific;
   Specific* mSpecific;
};
  
//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace


