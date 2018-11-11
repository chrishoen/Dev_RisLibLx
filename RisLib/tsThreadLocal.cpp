/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "tsThreadLocal.h"

namespace TS
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

ThreadLocal::ThreadLocal()
{
   strcpy(mThreadName, "NoThreadName");
   mCode = 0;
   mPrintLevel = 3;
   mLogLevel = 3;
}

void ThreadLocal::setThreadName(const char* aName)
{
   strncpy(mThreadName, aName,cMaxStringSize);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

