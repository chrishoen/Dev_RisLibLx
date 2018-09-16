//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <stdarg.h>

#include "risPortableCalls.h"
#include "my_functions.h"
#include "risAlphaDir.h"
#include "logFiles.h"

namespace Log
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Regional variables

static const int cMaxStringSize = 400;
static const int cMaxNumFiles = 200;

FILE* mFile [cMaxNumFiles];
bool  mTimestampEnable [cMaxNumFiles];

//******************************************************************************
//******************************************************************************
//******************************************************************************

void reset()
{
   for (int i = 0; i < cMaxNumFiles; i++)
   {
      mFile[i]=0;
      mTimestampEnable [i] = false;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool openFile(int aLogNum, char* aFileName)
{            
   char tBuf[400];
   mFile[aLogNum] = fopen(Ris::getAlphaFilePath_Log(tBuf,aFileName),"w");

   if (mFile[aLogNum]==0)
   {
      return false;
   }

   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool openFileAppend(int aLogNum, char* aFileName)
{            
   char tBuf[400];
   mFile[aLogNum] = fopen(Ris::getAlphaFilePath_Log(tBuf, aFileName), "a");

   if (mFile[aLogNum]==0)
   {
      return false;
   }

   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void closeFile(int aLogNum)
{            
   if (mFile[aLogNum] != 0)
   {
      fclose(mFile[aLogNum]);
   }
   mFile[aLogNum]=0;
}

void closeAllFiles()
{            
   for (int i = 0; i < cMaxNumFiles; i++)
   {
      if (mFile[i] != 0)
      {
         fclose(mFile[i]);
      }
      mFile[i]=0;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Write to the file

void write (int aLogNum, const char* aFormat, ...)
{
   //-----------------------------------------------------
   // Guard

   if (mFile[aLogNum]==0)
   {
      return;
   }

   //-----------------------------------------------------
   // Do a vsprintf with variable arg list into print string

   char tPrintString[cMaxStringSize];

   int tPrintSize;
   va_list  ArgPtr;
   va_start(ArgPtr, aFormat);
   tPrintSize = vsnprintf(tPrintString, cMaxStringSize, aFormat, ArgPtr);
   va_end(ArgPtr);

   tPrintString[tPrintSize++] = '\n';
   tPrintString[tPrintSize++] = 0;

   //-----------------------------------------------------
   // Print the string

   fputs(tPrintString,mFile[aLogNum]);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
} //namespace

