#ifndef _SOMETIMERTHREAD1_H_
#define _SOMETIMERTHREAD1_H_

/*==============================================================================
File: someThread1.h
Description:
QCall test thread class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risThreadsTimerThread.h"

namespace Some
{
//******************************************************************************
//******************************************************************************
//******************************************************************************

class TimerThread1 : public Ris::Threads::BaseTimerThread
{
public:
   typedef Ris::Threads::BaseTimerThread BaseClass;

   // Constructor
   TimerThread1();

   // Base class overloads
   void executeOnTimer(int aTimeCount);

   void showThreadInfo(int aTimeCount);

   // Members
   int mTimerFrequency;

};

//******************************************************************************
// Global instance

#ifdef _SOMETIMERTHREAD1_CPP_
          TimerThread1* gTimerThread1;
#else
   extern TimerThread1* gTimerThread1;
#endif

//******************************************************************************
}//namespace


#endif

