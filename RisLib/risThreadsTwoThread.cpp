/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "prnPrint.h"

#include "risThreadsTwoThread.h"

using namespace std;

//******************************************************************************
//******************************************************************************
//******************************************************************************

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
// Constructor.

TwoThreadShortThread::TwoThreadShortThread()
{
   mTimerPeriod = 0;
}

TwoThreadShortThread::~TwoThreadShortThread()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// BaseClass overload.

void TwoThreadShortThread::threadInitFunction() 
{
   // Call the call pointer.
   if (mThreadInitCallPointer) mThreadInitCallPointer();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// BaseClass overload.

void TwoThreadShortThread::threadExitFunction() 
{
   // Call the call pointer.
   if (mThreadExitCallPointer) mThreadExitCallPointer();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// BaseClass overload.

void TwoThreadShortThread::executeOnTimer(int aTimerCount)
{
   // Call the call pointer.
   if (mThreadExecuteOnTimerCallPointer) mThreadExecuteOnTimerCallPointer(aTimerCount);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

BaseTwoThread::BaseTwoThread()
{
   // Create thread objects.
   mShortThread = new TwoThreadShortThread;
   mLongThread  = new TwoThreadLongThread;

   mShortThread->setThreadPrintLevel(TS::PrintLevel(0, 3));
   mShortThread->setThreadPriority(Ris::Threads::gPriorities.mShort);
   mLongThread->mTimerPeriod = 0;
   
   mLongThread->setThreadPrintLevel(TS::PrintLevel(0, 3));
   mLongThread->setThreadPriority(Ris::Threads::gPriorities.mLong);
   mLongThread->mTimerPeriod = 0;

   mShutdownLongShort = true;

   // Bind qcall.
// mSendNotifyQCall.bind(mShortThread,this,&BaseTwoThread::executeSendNotify);
}

BaseTwoThread::~BaseTwoThread()
{
   delete mShortThread;
   delete mLongThread;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Launch

void BaseTwoThread::launchThreads()
{
   mLongThread->launchThread();
   mShortThread->launchThread();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Shutdown

void BaseTwoThread::shutdownThreads() 
{
   // Abort any pending waits.
   mLongThread->mQCallAbortFlag = true;
   mShortThread->mQCallAbortFlag = true;
   // Shutdown the threads.
   if (mShutdownLongShort)
   {
      mLongThread->shutdownThread();
      mShortThread->shutdownThread();
   }
   else
   {
      mShortThread->shutdownThread();
      mLongThread->shutdownThread();
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show thread state info.

void BaseTwoThread::showThreadInfo()
{
   mLongThread->showThreadInfo();
   mShortThread->showThreadInfo();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace
