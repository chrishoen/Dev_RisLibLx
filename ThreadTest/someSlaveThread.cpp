/*==============================================================================
File: someTest1Thread.cpp
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "someThreadParms.h"
#include "someMasterThread.h"

#define  _SOMESLAVETHREAD_CPP_
#include "someSlaveThread.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

SlaveThread::SlaveThread()
   : mRandomGen(mRandomDevice()),
     mRandomDis(gThreadParms.mDelayB1, gThreadParms.mDelayB2)
{
   // Set base class thread variables.
   BaseClass::setThreadName("SlaveThread");
   BaseClass::setThreadPrintLevel(TS::PrintLevel(3, 3));
   BaseClass::setThreadPriorityHigh();
   BaseClass::mTimerPeriod = 1000;

   // Set qcalls.
   mTest1QCall.bind(this, &SlaveThread::executeTest1);
   mWorkRequestQCall.bind(this, &SlaveThread::executeWorkRequest);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immediately 
// after the thread starts running.

void SlaveThread::threadInitFunction()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immediately
// before the thread is terminated.

void SlaveThread::threadExitFunction()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void SlaveThread::executeOnTimer(int aCurrentTimeCount)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void SlaveThread::executeWorkRequest (int aParm1,Ris::Threads::NotifyWrapper aCompletionNotify)
{
   int tDelay = mRandomDis(mRandomGen);
   BaseClass::threadSleep(tDelay);
   aCompletionNotify.notify();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void SlaveThread::executeTest1(int aSource, int aCode)
{
   printf("LINE101\n");
   Prn::print(Prn::View21, "SlaveThread::executeTest1 %d %d", aSource, aCode);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

}//namespace