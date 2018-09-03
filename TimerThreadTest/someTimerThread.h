#ifndef _SOMETIMERTHREAD_H_
#define _SOMETIMERTHREAD_H_

/*==============================================================================
File: someThread1.h
Description:
QCall test thread class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risThreadsTimerThread.h"
#include "risTimeMarker.h"

namespace Some
{
//******************************************************************************
//******************************************************************************
//******************************************************************************

class TimerThread : public Ris::Threads::BaseTimerThread
{
public:
   typedef Ris::Threads::BaseTimerThread BaseClass;

   // Constructor
   TimerThread();

   // Base class overloads
   void executeOnTimer(int aTimeCount);

   //--------------------------------------------------------------
   //--------------------------------------------------------------
   //--------------------------------------------------------------
   // Members:

   Ris::PeriodicTimeMarker mTimeMarker;

   bool mTPFlag;
   int  mTestCode;
   int  mFrequency;

   //--------------------------------------------------------------
   //--------------------------------------------------------------
   //--------------------------------------------------------------
   // Members:
   
   void executeTest1 (int aTimeCount);
   void executeTest2 (int aTimeCount);
};

//******************************************************************************
// Global instance

#ifdef _SOMETIMERTHREAD_CPP_
          TimerThread* gTimerThread;
#else
   extern TimerThread* gTimerThread;
#endif

//******************************************************************************
}//namespace


#endif

