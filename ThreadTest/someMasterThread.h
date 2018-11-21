#pragma once

/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risThreadsTwoThread.h"

namespace Some
{
//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is an example master thread that sends commands to a slave thread
// and waits for responses.

class MasterThread : public Ris::Threads::BaseTwoThread
{
public:
   typedef Ris::Threads::BaseTwoThread BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // If true the execute periodically.
   bool mTPFlag;

   // Metrics.
   int  mStatusCount1;
   int  mStatusCount2;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   MasterThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Thread base class overloads.

   // Thread init function. This is called by the base class immediately 
   // before the thread starts running.
   void threadInitFunction() override;

   // Thread exit function. This is called by the base class immediately 
   // after the thread starts running.
   void threadExitFunction() override;

   // Execute periodically. This is called by the base class timer.
   void executeOnTimer(int aTimerCount) override;


   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.qcalls.

   // Test qcall. It is invoked by the timer thread.
   Ris::Threads::QCall0 mTest0QCall;

   // Test function. This is bound to the qcall.
   void executeTest0();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.qcalls.

   // Test qcall. It is invoked by the timer thread.
   Ris::Threads::QCall2<int, int> mTest1QCall;

   // Test function. This is bound to the qcall.
   void executeTest1(int aSource, int aCode);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global instance.

#ifdef _SOMEMASTERTHREAD_CPP_
          MasterThread* gMasterThread;
#else
   extern MasterThread* gMasterThread;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
