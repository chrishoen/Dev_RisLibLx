/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risThreadsThreads.h"
#include "prnPrint.h"

#include "risThreadsTimerThread.h"
#include "risThreadsTimer.h"

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// class definition for implementation specific timer thread

class  TimerTimerThread : public Ris::Threads::BaseTimerThread
{
public:
   typedef Ris::Threads::BaseTimerThread BaseClass;

   TimerTimerThread()
   {
      mTimer = 0;
      mFirstFlag=true;
   }

   void configure(int aTimerPeriod, ThreadTimer* aTimer)
   {
      BaseClass::mTimerPeriod = aTimerPeriod;
      mTimer = aTimer;
   }

   //--------------------------------------------------------------
   // Thread base class overloads:

   void executeOnTimer(int aTimerCount)
   {
      if (mTimer==0) return;

      if (!mFirstFlag)
      {
         // Invoke user timer call
         mTimer->mTimerCall(aTimerCount);
      }
      mFirstFlag=false;
   }

   //--------------------------------------------------------------
   // Timer member
   ThreadTimer* mTimer;

   bool mFirstFlag;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// class definition for implementation specific

class ThreadTimer::Specific
{
public:
   Specific()
   {
      mTimerThread=0;
   }
   TimerTimerThread* mTimerThread;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

ThreadTimer::ThreadTimer()
{
   // Initialize members
   mTimerPeriod=1000;

   // Create new specific implementation
   mSpecific = new Specific;
}


ThreadTimer::~ThreadTimer()
{
   cancel();
   delete mSpecific;
}

//******************************************************************************
// Executes a timer call periodically, in milliseconds
void ThreadTimer::startTimer (TimerCall aTimerCall,int aTimerPeriod)
{
   mTimerCall    = aTimerCall;
   mTimerPeriod  = aTimerPeriod;

   // Create, configure, launch timer thread  
   mSpecific->mTimerThread = new TimerTimerThread();
   mSpecific->mTimerThread->configure(mTimerPeriod,this);
   mSpecific->mTimerThread->launchThread();
}

//******************************************************************************

void ThreadTimer::cancel()
{
   if (mSpecific->mTimerThread != 0)
   {
      mSpecific->mTimerThread->shutdownThread();
   }
}

}//namespace
}//namespace

