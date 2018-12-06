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
#include <functional>

#include "tsThreadServices.h"

#include "my_functions.h"
#include "risThreadsPriorities.h"
#include "prnPrint.h"
#include "risThreadsThreads.h"

namespace Ris
{
namespace Threads
{

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
// This is a c-function that is called if a thread is cancelled. It calls
// the thread exit function.

void BaseThread_Cleanup(void* argument)
{
   BaseThread* someThread = (BaseThread*)argument;
   TS::print(1, "threadExitFunction from cleanup BEGIN");
   someThread->threadExitFunction();
   TS::print(1, "threadExitFunction from cleanup END");
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
//******************************************************************************

BaseThread::BaseThread() 
{
   mBaseSpecific = new BaseSpecific;
   mBaseSpecific->mHandle = 0;
   mThreadRunState = 0;
   mThreadPriority = gPriorities.mNormal.mPriority;
   mThreadSingleProcessor = -1;
   mThreadStackSize = 0;
   mThreadRunProcessor = -1;


   // Create this now in the thread context of the thread creator.
   // It will be copied to the thread local storage variable at the
   // start of the thread run function.
   mThreadLocal = new TS::ThreadLocal;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

BaseThread::~BaseThread() 
{
   delete mBaseSpecific;
   delete mThreadLocal;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Set the thread services thread name in the thread local storage.

void BaseThread::setThreadName(const char* aThreadName)
{
   mThreadLocal->setThreadName(aThreadName);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Set the thread services print level in the thread local storage.

void BaseThread::setThreadPrintLevel(TS::PrintLevel aPrintLevel)
{
   mThreadLocal->mPrintLevel = aPrintLevel;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Set the thread processor number and priority.

void BaseThread::setThreadPriority(Priority aPriority)
{
   mThreadSingleProcessor = aPriority.mProcessor;
   mThreadPriority        = aPriority.mPriority;
}

void BaseThread::setThreadPriorityLow()
{
   mThreadSingleProcessor = gPriorities.mLow.mProcessor;
   mThreadPriority        = gPriorities.mLow.mPriority;
}

void BaseThread::setThreadPriorityNormal()
{
   mThreadSingleProcessor = gPriorities.mNormal.mProcessor;
   mThreadPriority        = gPriorities.mNormal.mPriority;
}

void BaseThread::setThreadPriorityHigh()
{
   mThreadSingleProcessor = gPriorities.mHigh.mProcessor;
   mThreadPriority        = gPriorities.mHigh.mPriority;
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
   // Do this first.

   TS::print(1, "");
   TS::print(1, "launchThread %s", mThreadLocal->mThreadName);

   // Set the run state.
   mThreadRunState = cThreadRunState_Launching;

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

   mThreadInitSem.get();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is the function that is executed in the context of the created thread.
// It calls a sequence of functions that are overloaded by inheritors.

void BaseThread::threadFunction()
{
   // Set the thread local storage pointer to the address of the
   // thread local storage object.
   TS::setThreadLocal(mThreadLocal);
   TS::print(1, "threadFunction BEGIN");

   // Set the processor that was current at the start of the thread
   // run function.
   mThreadRunProcessor = getThreadProcessorNumber();

   // Push the thread cleanup function. This is called if a thread is 
   // cancelled. It calls the thread exit function.
   pthread_cleanup_push(BaseThread_Cleanup, this);

   // Thread execution
   try
   {
      // Seed thread random number.
      my_srand();
      // This is used by inheritors to initialize resources. This should be
      // overloaded by thread base classes and not by thread user classes.
      mThreadRunState = cThreadRunState_InitR;
      threadResourceInitFunction();
      // Initialization section, overload provided by inheritors
      // It is intended that this will be overloaded by 
      // inheriting thread base classes and by inheriting user classes
      TS::print(1, "threadInitFunction");
      mThreadRunState = cThreadRunState_InitF;
      threadInitFunction();
      // Post to the thread init semaphore.
      mThreadInitSem.put();
      // Run section, overload provided by inheritors 
      TS::print(1, "threadRunFunction");
      mThreadRunState = cThreadRunState_Running;
      threadRunFunction();
      // Exit section, overload provided by inheritors
      TS::print(1, "threadExitFunction");
      mThreadRunState = cThreadRunState_ExitF;
      threadExitFunction();
      // This is used by inheritors to finalize resources. This should be
      // overloaded by thread base classes and not by thread user classes.
      mThreadRunState = cThreadRunState_ExitR;
      threadResourceExitFunction();
   }
   catch (char* aStr)
   {
      // Exception section, overload provided by inheritors
      threadExceptionFunction(aStr);
   }

   // Pop the thread cleanup function without executing it. This is the
   // normal execution path, where the thread is cancelled. If the thread
   // is cancelled, this does not execute.
   pthread_cleanup_pop(0);

   // Set the run state.
   mThreadRunState = cThreadRunState_Terminated;

   TS::print(1, "threadFunction END");
   // Zero the thread local storage pointer.
   TS::setThreadLocal(0);
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
   TS::print(1, "waitForThreadTerminate BEGIN %s", mThreadLocal->mThreadName);
   pthread_join(mBaseSpecific->mHandle,NULL);
   TS::print(1, "waitForThreadTerminate END   %s", mThreadLocal->mThreadName);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseThread::shutdownThreadPrologue()
{
   TS::print(1, "");
   TS::print(1, "shutdownThread %s", mThreadLocal->mThreadName);
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
   shutdownThreadPrologue();
   mTerminateFlag = true;
   waitForThreadTerminate();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseThreadWithTermSem::shutdownThread()
{
   shutdownThreadPrologue();
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

void BaseThread::showThreadFullInfo()
{
   printf("ThreadInfo>>>>>>>>>>>>>>>>>>>>>>>>>>BEGIN %s\n", mThreadLocal->mThreadName);

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

void BaseThread::showThreadInfo()
{
   int tThreadPriority = getThreadPriority();

   TS::print(0, "ThreadInfo %-20s %1d %3d %-8s %1d %1d",
      mThreadLocal->mThreadName,
      mThreadRunProcessor,
      tThreadPriority,
      asStringThreadRunState(),
      mThreadLocal->mPrintLevel.mOutLevel,
      mThreadLocal->mPrintLevel.mLogLevel);
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

char* BaseThread::asStringThreadRunState()
{
   switch (mThreadRunState)
   {
   case cThreadRunState_Launching:  return  "launching";
   case cThreadRunState_InitR:      return  "initR";
   case cThreadRunState_InitF:      return  "initF";
   case cThreadRunState_Running:    return  "running";
   case cThreadRunState_ExitF:      return  "exitF";
   case cThreadRunState_ExitR:      return  "exitR";
   case cThreadRunState_Terminated: return  "termed";
   default: return "UNKNOWN";
   }
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
