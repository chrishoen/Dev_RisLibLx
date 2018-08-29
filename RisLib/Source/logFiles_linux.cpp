/*==============================================================================
Print utility
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "risPortableCalls.h"
#include "logFiles.h"

namespace Log
{

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Regional variables

   static const int cMaxStringSize = 400;
   static const int cMaxNumFiles = 200;

   FILE* mFile [cMaxNumFiles];

//****************************************************************************
//****************************************************************************
//****************************************************************************

void reset()
{
   for (int i = 0; i < cMaxNumFiles; i++)
   {
      mFile[i]=0;
   }
}

//****************************************************************************
//****************************************************************************
//****************************************************************************

bool openFile(int aLogNum, char* aFileName)
{            
   mFile[aLogNum] = fopen(aFileName,"w");

   if (mFile[aLogNum]==0)
   {
      return false;
   }

   return true;
}

//****************************************************************************
//****************************************************************************
//****************************************************************************

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

   char tString[cMaxStringSize];

   int tPrintSize;
   va_list  ArgPtr;
   va_start(ArgPtr, aFormat);
   tPrintSize = vsnprintf(tString, cMaxStringSize, aFormat, ArgPtr);
   va_end(ArgPtr);

   tString[tPrintSize++] = '\n';
   tString[tPrintSize++] = 0;

   //-----------------------------------------------------
   // Print the string

   fputs(tString,mFile[aLogNum]);
}

} //namespace

