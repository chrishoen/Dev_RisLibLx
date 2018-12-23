//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <stdarg.h>

#include "my_functions.h"
#include "risThreadsThreads.h"
#include "risAlphaDir.h"

#include "tsShare.h"
#include "tsPrintThread.h"
#include "tsThreadServices.h"

namespace TS
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

void reset()
{
   strcpy(gShare.mProgramName, "SomeProgram");
}

void setProgramName(const char* aName)
{
   strncpy(gShare.mProgramName, aName, cMaxStringSize);
}

void setProgramLogFilepath(const char* aPath)
{
   strncpy(gShare.mProgramLogFilepath, aPath, cMaxStringSize);
}

void setProgramPrintLevel(PrintLevel aPrintLevel)
{
   gShare.mMainThreadLocal->mPrintLevel = aPrintLevel;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void initialize()
{
   // This executes in the context of the main thread, so set the thread
   // local storage pointer to the address of the main thread local storage
   // object.
   TS::setThreadLocal(gShare.mMainThreadLocal);

   // Disable prints.
   gShare.mPrintEnableFlag = false;

   // Launch the print thread.
   TS::gPrintThread = new TS::PrintThread;
   TS::gPrintThread->launchThread();

   // Let things settle.
// Ris::Threads::threadSleep(200);

   // Enable prints.
   gShare.mPrintEnableFlag = true;

   // Do a print.
   TS::print(1, "ThreadServices initialize");
}

void finalize()
{
   // Do a print.
   TS::print(1, "");
   TS::print(1, "ThreadServices finalize");

   // Let things settle.
// Ris::Threads::threadSleep(500);

   // shutdown the print thread.
   TS::gPrintThread->shutdownThread();
   delete TS::gPrintThread;

   // This executes in the context of the main thread, so set the thread
   // local storage pointer to zero.
   TS::setThreadLocal(0);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
} //namespace

