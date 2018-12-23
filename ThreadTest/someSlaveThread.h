#pragma once

/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <random>

#include "risThreadsQCallThread.h"
#include "risThreadsNotify.h"

namespace Some
{
//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is an example slave thread that receives commands from  a master 
// thread and sends responses.

class  SlaveThread : public Ris::Threads::BaseQCallThread
{
public:
   typedef Ris::Threads::BaseQCallThread BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Random number generator for random timer delay.
   std::random_device mRandomDevice;
   std::mt19937       mRandomGen;
   std::uniform_int_distribution<> mRandomDis;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   SlaveThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Thread init function. This is called by the base class immediately 
   // after the thread starts running.
   void threadInitFunction() override;

   // Thread exit function. This is called by the base class immediately
   // before the thread is terminated.
   void threadExitFunction() override;

   // Execute periodically. This is called by the base class timer.
   void executeOnTimer(int aCurrentTimeCount) override;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.qcalls.

   // Test qcall. It is invoked by the timer thread.
   Ris::Threads::QCall2<int, int> mTest1QCall;

   // Test function. This is bound to the qcall.
   void executeTest1(int aSource, int aCode);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Work request qcall from master thread.
   Ris::Threads::QCall2<int,Ris::Threads::NotifyWrapper> mWorkRequestQCall;

   // Work request function. This is bound to the qcall.
   void executeWorkRequest (int aParm1,Ris::Threads::NotifyWrapper aCompletionNotify);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global instance

#ifdef _SOMESLAVETHREAD_CPP_
          SlaveThread* gSlaveThread;
#else
   extern SlaveThread* gSlaveThread;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

