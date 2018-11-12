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
   BaseClass::setThreadPrintLevel(3);
   BaseClass::setThreadLogLevel(3);
   BaseClass::setThreadPriorityHigh();
   BaseClass::mTimerPeriod = 1000;

   // Set qcalls.
   mWorkRequestQCall.bind (this,&SlaveThread::executeWorkRequest);
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

void SlaveThread::executeWorkRequest (int aParm1,Ris::Threads::TwoThreadNotify aCompletionNotify)
{
   int tDelay = mRandomDis(mRandomGen);
   BaseClass::threadSleep(tDelay);
   aCompletionNotify.notify();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

}//namespace