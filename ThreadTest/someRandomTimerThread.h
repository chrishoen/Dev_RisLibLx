#pragma once

/*==============================================================================
Some timer thread class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <random>
#include "risThreadsThreads.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a test timer thread that drives the test qcall thread.
//   

class RandomTimerThread : public Ris::Threads::BaseThreadWithTermFlag
{
public:

   typedef Ris::Threads::BaseThreadWithTermFlag BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Ident.
   int mIdent;

   // If true then execute periodic function.
   bool mTPFlag;

   // Random number generator for random timer delay.
   std::random_device mRandomDevice;
   std::mt19937       mRandomGen;
   std::uniform_int_distribution<> mRandomDis;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   RandomTimerThread(int aIdent);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Thread base class overloads.

   // Thread init function. This is called by the base class immediately 
   // after the thread starts running.
   void threadInitFunction() override;

   // Thread run function. This is called by the base class immediately 
   // after the thread init function. It runs a loop that sends a qcall
   // to the qcall test thread and waits for a random time.
   void threadRunFunction() override;

   // Thread exit function. This is called by the base class immediately
   // before the thread is terminated.
   void threadExitFunction() override;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. 
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global singular instance.

#ifdef _SOMETIMERTHREAD_CPP_
         RandomTimerThread* gRandomTimerThread1;
         RandomTimerThread* gRandomTimerThread2;
#else
extern   RandomTimerThread* gRandomTimerThread1;
extern   RandomTimerThread* gRandomTimerThread2;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
