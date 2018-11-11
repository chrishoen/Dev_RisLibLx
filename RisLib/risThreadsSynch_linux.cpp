/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include "my_functions.h"
#include "prnPrint.h"
#include "risNanoTime.h"
#include "risThreadsSynch.h"


namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
class BinarySemaphore::Specific
{
public:
   sem_t mHandle;
};

//******************************************************************************

BinarySemaphore::BinarySemaphore() 
{
   mSpecific = new Specific;
   sem_init(&mSpecific->mHandle,0,0);
   mStatusCode=0;
}

//******************************************************************************

BinarySemaphore::~BinarySemaphore() 
{
   sem_destroy(&mSpecific->mHandle);
   delete mSpecific;
}

//******************************************************************************

void BinarySemaphore::reset()
{
   sem_init(&mSpecific->mHandle,0,0);
}

//******************************************************************************

void BinarySemaphore::put()
{
   // Get current semaphore count
   int tCount=0;
   sem_getvalue(&mSpecific->mHandle, &tCount); 

   // If not zero then return, this is a binary semaphore
   if (tCount != 0) return;

   // Post to semahore
   sem_post(&mSpecific->mHandle);
}

//******************************************************************************

bool BinarySemaphore::get(int aTimeout)
{
   switch (aTimeout)
   {
   case 0 :
      return true;
      break;
   case -1 :
      sem_wait(&mSpecific->mHandle);
      return true;
      break;
   default :
      // Time variables
      timespec   tBeginTimespec;
      timespec   tEndTimespec;
      timespec   tExpireTimespec;
      long long  tBeginTimeNs;
      long long  tEndTimeNs;
      long long  tExpireTimeNs;

      // Get current timespec at beginning
      clock_gettime(CLOCK_REALTIME, &tBeginTimespec);

      // Convert to ns
      Ris::NanoTime::getNsFromTimespec(tBeginTimeNs,&tBeginTimespec);

      // Calculate expiration time from timeout (which is in milliseconds)
      tExpireTimeNs = tBeginTimeNs + Ris::NanoTime::getNsFromMs(aTimeout);

      // Convert to timespec
      Ris::NanoTime::getTimespecFromNs(&tExpireTimespec,tExpireTimeNs);

      // Semaphore timed wait with expiration timespec
      sem_timedwait(&mSpecific->mHandle,&tExpireTimespec);

      // Get current timespec at end
      clock_gettime(CLOCK_REALTIME, &tEndTimespec);

      // Convert to ns
      Ris::NanoTime::getNsFromTimespec(tEndTimeNs,&tEndTimespec);

      // Compare
      bool tNoTimeout = tExpireTimeNs >= tEndTimeNs ? true : false; 

      // Result
      return tNoTimeout;
   }
   return false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

class CountingSemaphore::Specific
{
public:
   sem_t mHandle;
};

//******************************************************************************

CountingSemaphore::CountingSemaphore() 
{
   mSpecific = new Specific;
   sem_init(&mSpecific->mHandle,0,0);
}

CountingSemaphore::CountingSemaphore(int aInitial) 
{
   mSpecific = new Specific;
   sem_init(&mSpecific->mHandle,0,aInitial);
}

//******************************************************************************

CountingSemaphore::~CountingSemaphore() 
{
   sem_destroy(&mSpecific->mHandle);
   delete mSpecific;
}

//******************************************************************************

void CountingSemaphore::put()
{
   sem_post(&mSpecific->mHandle);
}

//******************************************************************************

bool CountingSemaphore::get(int timeout)
{
   return sem_wait(&mSpecific->mHandle) == 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

class MutexSemaphore::Specific
{
public:
   pthread_mutex_t mMutex;
};

//******************************************************************************

MutexSemaphore::MutexSemaphore() 
{
   mSpecific = new Specific;
   pthread_mutex_init(&mSpecific->mMutex, NULL);
}

//******************************************************************************

MutexSemaphore::~MutexSemaphore() 
{
   pthread_mutex_destroy(&mSpecific->mMutex);
   delete mSpecific;
}

//******************************************************************************

void MutexSemaphore::lock()
{
   pthread_mutex_lock(&mSpecific->mMutex);
}

//******************************************************************************

void MutexSemaphore::unlock()
{
   pthread_mutex_unlock(&mSpecific->mMutex);
}

}//namespace
}//namespace


