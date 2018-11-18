/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "tsThreadServices.h"

#define  _TSSHARE_CPP_
#include "tsShare.h"

namespace TS
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

Share::Share()
{
   strcpy(mProgramName, "SomeProgram");
   mProgramLogFilepath[0]=0;

   // Create this now in the thread context of the thread creator.
   // It will be copied to the thread local storage variable at the
   // start of the thread run function.
   mMainThreadLocal = new TS::ThreadLocal;
   mMainThreadLocal->setThreadName("main");

   mPrintEnableFlag = false;
}

Share::~Share()
{
   delete mMainThreadLocal;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
