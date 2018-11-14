/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "ris_priorities.h"
#include "risAlphaDir.h"
#include "tsShare.h"

#define  _TSPRINTTHREAD_CPP_
#include "tsPrintThread.h"

namespace TS
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

PrintThread::PrintThread()
{
   // Set base class thread parameters.
   BaseClass::setThreadName("TSPrint");
   BaseClass::setThreadPrintLevel(3);
   BaseClass::setThreadLogLevel(0);
   BaseClass::mTimerPeriod = 0;
   BaseClass::mThreadPriority = Ris::get_default_print_thread_priority();

   // Initialize qcalls.
   mWriteQCall.bind(this, &PrintThread::executeWrite);

   // Initialize variables.
   mFile = 0;
   mWriteCount = 0;
}

PrintThread::~PrintThread()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
void PrintThread::doFileOpenNew()
{
   char tBuf[400];
   char tFileName[400];
   strcpy(tFileName, gShare.mProgramName);
   strcat(tFileName, "Log.txt");

   mFile = fopen(Ris::getAlphaFilePath_Log(tBuf, tFileName), "w");
}

void PrintThread::doFileOpenAppend()
{
   char tBuf[400];
   char tFileName[400];
   strcpy(tFileName, gShare.mProgramName);
   strcat(tFileName, "Log.txt");

   mFile = fopen(Ris::getAlphaFilePath_Log(tBuf, tFileName), "w+");
}

void PrintThread::doFileClose()
{
   if (mFile)
   {
      fclose(mFile);
      mFile = 0;
   }
}

void PrintThread::doFileFlush()
{
   fflush(mFile);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immediately 
// after the thread starts running. This opens the file.

void PrintThread::threadInitFunction()
{
   // Open the log file.
   doFileOpenNew();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function, base class overload. This closes the file.

void  PrintThread::threadExitFunction()
{
   // Close the file.
   doFileClose();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Write function. This is bound to the qcall. It writes a string to the
// log file and then deletes it

void PrintThread::executeWrite(PrintString* aString)
{
   // Guard.
   if (!gShare.mPrintEnableFlag)
   {
      delete aString;
      return;
   }

   // Print to stdout.
   fputs(aString->mString, stdout);

   // Print to the log file.
   fputs(aString->mString, mFile);
   if (ferror(mFile))
   {
      printf("fputs error1\n");
      perror("fputs error2\n");
   }

   // Done.
   delete aString;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace