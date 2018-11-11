//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <stdarg.h>

#include "my_functions.h"
#include "risAlphaDir.h"

#include "tsShare.h"
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

void setProgramPrintLevel(int aPrintLevel)
{
   gShare.mMainThreadLocal->mPrintLevel = aPrintLevel;
   gShare.mMainThreadLocal->mLogLevel = 3;
}

void setProgramLogLevel(int aLogLevel)
{
   gShare.mMainThreadLocal->mLogLevel = aLogLevel;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool openLogFile()
{            
   char tBuf[400];
   char tFileName[400];
   strcpy(tFileName, gShare.mProgramName);
   strcat(tFileName,".txt");

   gShare.mLogFile = fopen(Ris::getAlphaFilePath_Log(tBuf,tFileName),"w");

   if (gShare.mLogFile==0)
   {
      return false;
   }

   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void closeLogFile()
{
   if (gShare.mLogFile != 0)
   {
      fclose(gShare.mLogFile);
   }
   gShare.mLogFile = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void initialize()
{
   openLogFile();

   // This executes in the context of the main thread, so set the thread
   // local storage pointer to the address of the main thread local storage
   // object.
   TS::setThreadLocal(gShare.mMainThreadLocal);
   TS::print(1, "ThreadServices initialize");
}

void finalize()
{
   openLogFile();
   // This executes in the context of the main thread, so set the thread
   // local storage pointer to zero.
   TS::setThreadLocal(0);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
} //namespace

