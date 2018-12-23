/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "cmnPriorities.h"
#include "someThreadParms.h"
#include "someSlaveThread.h"

#define  _SOMEMASTERTHREAD_CPP_
#include "someMasterThread.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

MasterThread::MasterThread()
{
   using namespace std::placeholders;

   // Set base class thread variables.
   BaseClass::mLongThread->setThreadName("MasterLong");
   BaseClass::mLongThread->setThreadPriority(Cmn::gPriorities.mMasterLong);
   BaseClass::mLongThread->setThreadPrintLevel(TS::PrintLevel(3, 3));

   BaseClass::mShortThread->setThreadName("MasterShort");
   BaseClass::mShortThread->setThreadPriority(Cmn::gPriorities.mMasterShort);
   BaseClass::mShortThread->setThreadPrintLevel(TS::PrintLevel(3, 3));
   BaseClass::mShortThread->mTimerPeriod = 1000;

   // Set base class call pointers.
   BaseClass::mShortThread->mThreadInitCallPointer           = std::bind(&MasterThread::threadInitFunction, this);
   BaseClass::mShortThread->mThreadExitCallPointer           = std::bind(&MasterThread::threadExitFunction, this);
   BaseClass::mShortThread->mThreadExecuteOnTimerCallPointer = std::bind(&MasterThread::executeOnTimer, this, _1);

   // Set qcalls.
   mTest0QCall.bind(this->mLongThread, this, &MasterThread::executeTest0);
   mTest1QCall.bind(this->mLongThread, this, &MasterThread::executeTest1);

   // Set member variables.
   mTPFlag = true;
   mStatusCount1 = 0;
   mStatusCount2 = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immediately 
// before the thread starts running.

void MasterThread::threadInitFunction()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immediately 
// after  the thread starts running.

void MasterThread::threadExitFunction()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Execute periodically. This is called by the base class timer.

void MasterThread::executeOnTimer(int aTimerCount)
{
   if (!mTPFlag) return;
// Prn::print(Prn::View11, "StatusCount %10d", mStatusCount1);
   Prn::print(Prn::View11, "StatusCount %10d %10d", aTimerCount,mStatusCount1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void MasterThread::executeTest0()
{
   Prn::print(Prn::View21, "MasterThread::executeTest0");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void MasterThread::executeTest1(int aSource, int aCode)
{
   try
   {
      Prn::print(Prn::View22, "MasterThread::executeTest1 BEGIN", aCode);

      mNotify.setMaskOne("SlaveCompletion", 1);
      gSlaveThread->mWorkRequestQCall(aCode, Ris::Threads::NotifyWrapper(&mNotify, 1));
      mNotify.wait(-1);

      Prn::print(Prn::View22, "MasterThread::executeTest1 END");
   }
   catch (int aException)
   {
      Prn::print(Prn::View11, "EXCEPTION TestQCallThread::executeTest1 %d %s", aException, mNotify.mException);
   }

   if (gThreadParms.mShowCode == 1 || aSource == 7)
   {
      Prn::print(Prn::View21, "MasterThread::executeTest1 %d %d", aSource, aCode);
   }
   mStatusCount1++;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace