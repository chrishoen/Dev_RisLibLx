/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "someThreadParms.h"
#include "someMasterThread.h"

#define  _SOMETIMERTHREAD_CPP_
#include "someRandomTimerThread.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

RandomTimerThread::RandomTimerThread(int aIdent)
   : mIdent(aIdent),
     mRandomGen(mRandomDevice()),
     mRandomDis(gThreadParms.mDelayA1, gThreadParms.mDelayA2)
{
   // Set base class variables.
   if (mIdent == 1)
   {
      BaseClass::setThreadName("RandomTimer1");
      BaseClass::setThreadPrintLevel(3);
   }
   else
   {
      BaseClass::setThreadName("RandomTimer2");
      BaseClass::setThreadPrintLevel(2);
   }

   mTPFlag = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immediately 
// after the thread starts running.

void RandomTimerThread::threadInitFunction()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function. This is called by the base class immediately 
// after the thread init function. It runs a loop that waits on SDL
// events and processes posted events. The loop exits when it receives
// a quit event.

void RandomTimerThread::threadRunFunction()
{
   // Loop to wait for posted events and process them.
   int tCount = 0;
   while (true)
   {
      // Test for thread termination.
      if (BaseClass::mTerminateFlag) break;

      // Wait for a random delay.
      int tDelay = mRandomDis(mRandomGen);
      Prn::print(Prn::ThreadRun4, "Delay %4d", tDelay);
      BaseClass::threadSleep(tDelay);

      // Send a qcall to the test thread.
      if (mTPFlag) executeOnTimer(tCount);
      tCount++;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immediately
// before the thread is terminated.

void RandomTimerThread::threadExitFunction()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void RandomTimerThread::executeOnTimer(int aCount)
{
// TS::print(0, "print %d", aCount);

   gMasterThread->mTest1QCall(mIdent, aCount);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace