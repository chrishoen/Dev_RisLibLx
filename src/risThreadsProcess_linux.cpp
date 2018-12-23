/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include <signal.h>
#include <pthread.h>
#include <sched.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <assert.h>


#include "risThreadsProcess.h"

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

static int mTimerPeriod = 10;

void setProcessTimerResolution(int aTimerPeriod)
{
   mTimerPeriod = aTimerPeriod;
}

int  getProcessTimerResolution()
{
   return mTimerPeriod;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void enterProcessHigh()
{
   signal(SIGPIPE, SIG_IGN);

   return;
   sched_param param;
   param.sched_priority = sched_get_priority_max(SCHED_FIFO);
   int ret = sched_setscheduler(getpid(), SCHED_FIFO, &param);
   if (ret) printf("sched_setschedparam ERROR %d\n", errno);
}

void exitProcess()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void setProcessAffinityMask(unsigned aMask)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void showCurrentThreadInfo()
{
   int tCurrentProcessorNumber = sched_getcpu();;

   sched_param param;
   int policy;
   pthread_getschedparam(pthread_self(), &policy, &param);
   int tThreadPriority = param.sched_priority;

   TS::print(1, "");
   TS::print(0, "ThreadInfo %-20s %1d %3d %-8s %1d %1d",
      "main",
      tCurrentProcessorNumber,
      tThreadPriority,
      "running",
      TS::tls()->mPrintLevel.mOutLevel,
      TS::tls()->mPrintLevel.mLogLevel);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

