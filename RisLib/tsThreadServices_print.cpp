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
   if (!isEnabled()) return;
   if (aLevel > tls()->mPrintLevel && aLevel > tls()->mLogLevel) return;

   //*************************************************************************
   //*************************************************************************
   //*************************************************************************
   // Guard against having too many prints that are at or above level 4.

   if (aLevel >= 4)
   {
      // Increment.
      tls()->mPrintCount4++;

      // If at the limit then print a message and exit.
      if (tls()->mPrintCount4 == 40)
      {
         fputs("PRINT COUNT 4 LIMIT HAS BEEN REACHED\n", stdout);
         if (gShare.mLogFile)
         {
            fputs("PRINT COUNT 4 LIMIT HAS BEEN REACHED\n", gShare.mLogFile);
         }
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
   char  tPrintString[cMaxStringSize];
   int   tPrintSize;

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

   tPrintSize = sprintf(tPrintString,"%-20s $$ %s\n",
      tls()->mThreadName,
      tInputString);

   //*************************************************************************
   //*************************************************************************
   //*************************************************************************
   // Print the string.

   // Print to stdout.
   if (aLevel <= tls()->mPrintLevel)
   {
      fputs(tPrintString, stdout);
   }

   // Print to the log file.
   if (gShare.mLogFile && aLevel <= tls()->mLogLevel)
   {
      fputs(tPrintString, gShare.mLogFile);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
} //namespace

