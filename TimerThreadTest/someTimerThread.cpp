/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "risProgramTime.h"

#define  _SOMETIMERTHREAD_CPP_
#include "someTimerThread.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

TimerThread::TimerThread()
{
   // Set base class thread parameters.
   BaseClass::setThreadPriorityHigh();
   BaseClass::mThreadSingleProcessor = 2;

   // Set timer period
   mFrequency = 20;
   BaseClass::mTimerPeriod = 1000 / mFrequency;

   mTimeMarker.initialize(5*mFrequency);

   // Members
   mTPFlag = false;
   mTestCode = 3;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TimerThread::threadInitFunction()
{
   if (mTestCode !=3) return;

   BaseClass::showThreadFullInfo();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TimerThread::executeOnTimer(int aTimeCount)
{
   if (aTimeCount < mFrequency) return;

   switch (mTestCode)
   {
      case 1: executeTest1 (aTimeCount); break;
      case 2: executeTest2 (aTimeCount); break;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TimerThread::executeTest1(int aTimeCount)
{
   mTimeMarker.doStop();
   mTimeMarker.doStart();

   if (mTimeMarker.mStatistics.mEndOfPeriod)
   {
      double tPeriodUS = mTimerPeriod*1000.0;
//    tPeriodUS = 0.0;
      Prn::print(Prn::ThreadRun1, "TEST %5d $$ %10.3f  %10.3f  %10.3f  %10.3f",
         mTimeMarker.mStatistics.mSize,
         mTimeMarker.mStatistics.mMean   - tPeriodUS,
         mTimeMarker.mStatistics.mStdDev,
         mTimeMarker.mStatistics.mMinX   - tPeriodUS,
         mTimeMarker.mStatistics.mMaxX   - tPeriodUS);
         mTimeMarker.mStatistics.mEndOfPeriod = false;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TimerThread::executeTest2(int aTimeCount)
{
   Prn::print(Prn::ThreadRun1, "TEST2 %5d $$ %3d %10.4f",
      aTimeCount,
      BaseClass::getThreadProcessorNumber(),
      Ris::getCurrentProgramTime());
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

}//namespace