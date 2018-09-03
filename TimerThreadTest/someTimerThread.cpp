/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#define  _SOMETIMERTHREAD_CPP_
#include "someTimerThread.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

TimerThread::TimerThread()
{
   // Set base class thread priority
   BaseClass::setThreadPriorityHigh();

   mFrequency = 20;

   // Set timer period
   BaseClass::mTimerPeriod = 1000 / mFrequency;

   mTimeMarker.initialize(5*mFrequency);

   // Members
   mTPFlag = false;
   mTestCode = 1;
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
// Prn::print(Prn::ThreadRun2, "executeOnTimer %d",aTimeCount);

   //---------------------------------------------------------------------------
   // Time marker

   mTimeMarker.doStop();
   mTimeMarker.doStart();

   if (mTimeMarker.mStatistics.mEndOfPeriod)
   {
      double tPeriodUS = mTimerPeriod*1000.0;
//    tPeriodUS=0.0;
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
   int tCount = 10000;

   Ris::Threads::MutexSemaphore tMutex;

   mTimeMarker.initialize(tCount);

   while(true)
   {
      mTimeMarker.doStart();

      tMutex.lock();
      tMutex.unlock();

      mTimeMarker.doStop();

      if (mTimeMarker.mStatistics.mEndOfPeriod)
      {
         break;
      }
   }

   Prn::print(Prn::ThreadRun1, "TEST2 %5d $$ %10.3f  %10.3f  %10.3f  %10.3f",
      aTimeCount,
      mTimeMarker.mStatistics.mMean,
      mTimeMarker.mStatistics.mStdDev,
      mTimeMarker.mStatistics.mMinX,
      mTimeMarker.mStatistics.mMaxX);
}

}//namespace