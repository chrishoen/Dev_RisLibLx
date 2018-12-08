//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <stdarg.h>

#include "my_functions.h"
#include "risAlphaDir.h"

#include "tsShare.h"
#include "tsPrintThread.h"
#include "tsThreadServices.h"

namespace TS
{

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Print. This is input a print level 0,1,2 and the signature for a
// printf. If the input level is less than or equal to the print level
// that is located in thread local storage then the print is performed
// else it is ignored. If the print is performed it prints a string
// to stdout and and to the log file.

void print(int aLevel, const char* aFormat, ...)
{
   //*************************************************************************
   //*************************************************************************
   //*************************************************************************
   // Do this first.

   // Guard.
   if (!gShare.mPrintEnableFlag) return;
   if (gPrintThread == 0) return;
   if (!isEnabled()) return;
   if (aLevel > tls()->mPrintLevel.mOutLevel && 
       aLevel > tls()->mPrintLevel.mLogLevel) return;

   //*************************************************************************
   //*************************************************************************
   //*************************************************************************
   // Guard against having too many prints that are at or above level 4.

   if (aLevel >= 4 && false)
   {
      // Increment.
      tls()->mPrintCount4++;

      // If at the limit then print a message and exit.
      if (tls()->mPrintCount4 == 40)
      {
         PrintString* tPrintString = new PrintString("PRINT COUNT 4 LIMIT HAS BEEN REACHED");
         tPrintString->mOutFlag = true;
         tPrintString->sendToPrintThread();
         return;
      }

      // If above the limit then exit.
      if (tls()->mPrintCount4 > 40) return;
   }

   //*************************************************************************
   //*************************************************************************
   //*************************************************************************
   // Strings.

   // Input string buffer.
   char  tInputString[cMaxStringSize];
   int   tInputSize;

   // Print string buffer.
   char  tOutputString[cMaxStringSize];
   int   tOutputSize;

   //*************************************************************************
   //*************************************************************************
   //*************************************************************************
   // Do a vsprintf with variable arg list into the input string buffer.

   va_list  ArgPtr;
   va_start(ArgPtr, aFormat);
   tInputSize = vsnprintf(tInputString, cMaxStringSize, aFormat, ArgPtr);
   va_end(ArgPtr);

   //*************************************************************************
   //*************************************************************************
   //*************************************************************************
   // Do an sprintf with the thread name and the input string into the
   // print string. Append a newline \n.

   tOutputSize = sprintf(tOutputString,"%-20s $$ %s\n",
      tls()->mThreadName,
      tInputString);

   //*************************************************************************
   //*************************************************************************
   //*************************************************************************
   // Print the string.

   // Create a new string instance and send it to the print thread.
   PrintString* tPrintString = new PrintString(tOutputString);
   tPrintString->mOutFlag = aLevel <= tls()->mPrintLevel.mOutLevel;
   tPrintString->sendToPrintThread();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
} //namespace

