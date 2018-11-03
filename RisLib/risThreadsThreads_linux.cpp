/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <pthread.h>
#include <sched.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <assert.h>

#include "my_functions.h"
#include "ris_priorities.h"
#include "prnPrint.h"
#include "risThreadsThreads.h"

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a c-function that is used to execute the thread function of 
// a base thread object. It is passed to CreateThread to indirectly call
// the thread function because CreateThread can't use member function
// addresses.

void* BaseThread_Execute (void* argument)
{
   BaseThread* someThread = (BaseThread*)argument;
   someThread->threadFunction();
   return 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

class BaseThread::BaseSpecific
{
public:
   pthread_t mHandle;
};

//******************************************************************************
//******************************************************************************

BaseThread::BaseThread() 
{
   mBaseSpecific = new BaseSpecific;
   mBaseSpecific->mHandle    = 0;
   mThreadPriority = get_default_thread_priority();
   mThreadSingleProcessor  = -1;

   mThreadStackSize = 0;
   mThreadInitSemFlag = true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

BaseThread::~BaseThread() 
{
   delete mBaseSpecific;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void* BaseThread::getHandlePtr()
{
   return (void*)&mBaseSpecific->mHandle;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseThread::configureThread()
{
   mBaseSpecific->mHandle = 0;
   mThreadStackSize = 0;
}

void BaseThread::setThreadPriorityHigh()
{
   mThreadPriority = get_default_high_thread_priority();
}

void BaseThread::setThreadPriorityLow()
{
   mThreadPriority = get_default_low_thread_priority();
}

void BaseThread::setThreadPriority(int aThreadPriority)
{
   mThreadPriority = aThreadPriority;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void chkerror(int aRet, char* aLabel)
{
   if (aRet == 0)return;
   printf("FAIL %s %d\n", aLabel,aRet);
   exit(1);
}

void BaseThread::launchThread()
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Configure thread parameters, but do not make any OS calls

   configureThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Thread attributes.

   // Thread attributes, initialize.
   pthread_attr_t tAttributes;
   pthread_attr_init(&tAttributes);
   
   int ret;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Thread attributes, thread priority.

   ret = pthread_attr_setscope(&tAttributes, PTHREAD_SCOPE_SYSTEM);
   chkerror(ret, "pthread_attr_setscope");

   ret = pthread_attr_setinheritsched(&tAttributes, PTHREAD_EXPLICIT_SCHED);
   chkerror(ret, "pthread_attr_setinheritsched");

   ret = pthread_attr_setschedpolicy(&tAttributes, SCHED_FIFO);
   chkerror(ret, "pthread_attr_setschedpolicy");

   sched_param tSchedParam;
   tSchedParam.sched_priority = mThreadPriority;
   ret = pthread_attr_setschedparam(&tAttributes, &tSchedParam);
   chkerror(ret, "pthread_attr_setschedparam");

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Thread attributes, affinity mask.

   if (mThreadSingleProcessor >= 0)
   {
      cpu_set_t tAffinityMask;
      CPU_ZERO(&tAffinityMask);
      CPU_SET(mThreadSingleProcessor, &tAffinityMask);
      ret = pthread_attr_setaffinity_np(&tAttributes, sizeof(tAffinityMask), &tAffinityMask);
      chkerror(ret, "pthread_attr_setaffinity_np");
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Create the thread. The following threadRunFunction will execute
   // in the context of the created thread.

   ret = pthread_create(
      &mBaseSpecific->mHandle,
      &tAttributes,
      &BaseThread_Execute,
      (void*)this);
   chkerror(ret, "pthread_create");

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Thread attributes, finalize.

   pthread_attr_destroy(&tAttributes);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Wait for the thread init function to complete.

   if (mThreadInitSemFlag)
   {
      mThreadInitSem.get();
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is the function that is executed in the context of the created thread.
// It calls a sequence of functions that are overloaded by inheritors.

void BaseThread::threadFunction()
{
   // Thread execution
   try
   {
      // Seed thread random number.
      my_srand();
      // This is used by inheritors to initialize resources. This should be
      // overloaded by thread base classes and not by thread user classes.
      threadResourceInitFunction();
      // Initialization section, overload provided by inheritors
      // It is intended that this will be overloaded by 
      // inheriting thread base classes and by inheriting user classes
      threadInitFunction();
      // It is intended that this will be overloaded by 
      // inheriting thread base classes that provide timers,
      // and not by inheriting user classes.
      // Note that the timer starts after the initialization section
      // has completed 
      threadTimerInitFunction();
      // Post to the thread init semaphore.
      if (mThreadInitSemFlag)
      {
         mThreadInitSem.put();
      }
      // Run section, overload provided by inheritors 
      threadRunFunction();
      // Exit section, overload provided by inheritors
      threadExitFunction();
      // This is used by inheritors to finalize resources. This should be
      // overloaded by thread base classes and not by thread user classes.
      threadResourceExitFunction();
   }
   catch (char* aStr)
   {
      // Exception section, overload provided by inheritors
      threadExceptionFunction(aStr);
   }
   catch (...)
   {
      // Exception section, overload provided by inheritors
      threadExceptionFunction("UNKNOWN");
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseThread::threadExceptionFunction(char* aStr)
{
   Prn::print(0,"BaseThread::threadExceptionFunction %s",aStr);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseThread::threadSleep(int aTicks)
{
   // Calculate timespec variables
   int tMs  = aTicks;
   int tSec = tMs / 1000;
   int tNs  = (tMs - tSec*1000)*1000000;
   if (tNs>999999999) tNs=999999999;

   timespec tTime;
   tTime.tv_sec=tSec;
   tTime.tv_nsec = tNs;

   // Nanosleep
   nanosleep(&tTime,&tTime);
 //sleep(aTicks);  
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseThread::forceTerminateThread()
{
   pthread_cancel(mBaseSpecific->mHandle);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseThread::waitForThreadTerminate()
{
   pthread_join(mBaseSpecific->mHandle,NULL);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

BaseThreadWithTermFlag::BaseThreadWithTermFlag() 
{
   mTerminateFlag = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseThreadWithTermFlag::shutdownThread()
{
   mTerminateFlag = true;
   waitForThreadTerminate();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseThreadWithTermSem::shutdownThread()
{
   mTerminateSem.put();
   waitForThreadTerminate();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

int BaseThread::getThreadPriority()
{
   sched_param param;
   int policy;

   pthread_getschedparam(mBaseSpecific->mHandle, &policy, &param);

   return param.sched_priority;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

int BaseThread::getThreadProcessorNumber()
{
   return sched_getcpu();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is the function that is executed in the context of the created thread.
// It calls a sequence of functions that are overloaded by inheritors.

void BaseThread::threadShowInfo(char* aLabel)
{
   printf("ThreadInfo>>>>>>>>>>>>>>>>>>>>>>>>>>BEGIN %s\n", aLabel);

   int tMaxPriority = sched_get_priority_max(SCHED_FIFO);
   int tMinPriority = sched_get_priority_min(SCHED_FIFO);

   sched_param tThreadSchedParam;
   int tThreadPolicy = 0;
   pthread_getschedparam(mBaseSpecific->mHandle, &tThreadPolicy, &tThreadSchedParam);
   int tThreadPriority = tThreadSchedParam.__sched_priority;

   int tNumProcessors = sysconf(_SC_NPROCESSORS_ONLN);
   int tCurrentProcessorNumber = sched_getcpu();

   cpu_set_t tAffinityMask;
   pthread_getaffinity_np(mBaseSpecific->mHandle, sizeof(tAffinityMask), &tAffinityMask);
   unsigned tUMask = 0;
   for (int i = 0; i < 32; i++) if (CPU_ISSET(i, &tAffinityMask)) (tUMask |= (1 << i));

   printf("NumProcessors           %8d\n", tNumProcessors);
   printf("MaxPriority             %8d\n", tMaxPriority);
   printf("MinPriority             %8d\n", tMinPriority);
   printf("\n");
   printf("ThreadPolicy            %8d\n", tThreadPolicy);
   printf("ThreadPriority          %8d\n", tThreadPriority);
   printf("mThreadPriority         %8d\n", mThreadPriority);
   printf("\n");
   printf("ThreadAffinityMask      %8X\n", tUMask);
   printf("mThreadSingleProcessor  %8X\n", mThreadSingleProcessor);
   printf("CurrentProcessorNumber  %8d\n", tCurrentProcessorNumber);

   printf("ThreadInfo<<<<<<<<<<<<<<<<<<<<<<<<<<END\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

int countsPerOneSecond()
{
   return 1000;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void threadSleep(int aTicks)
{
   // Calculate timespec variables
   int tMs = aTicks;
   int tSec = tMs / 1000;
   int tNs = (tMs - tSec * 1000) * 1000000;
   if (tNs > 999999999) tNs = 999999999;

   timespec tTime;
   tTime.tv_sec = tSec;
   tTime.tv_nsec = tNs;

   // Nanosleep
   nanosleep(&tTime, &tTime);
   //sleep(aTicks);  
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void halt(char* aPrintStr)
{
   printf("HALTING %s\n", aPrintStr);
   exit(1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace
