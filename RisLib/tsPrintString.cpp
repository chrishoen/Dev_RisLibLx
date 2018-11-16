/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <new>
#include <stdarg.h>
#include <ctype.h>

#include "my_functions.h"

#include "tsShare.h"
#include "tsPrintThread.h"
#include "tsPrintString.h"

namespace TS
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor

PrintString::PrintString()
{
   mPrintFlag = false;
   mString[0] = 0;
}

PrintString::PrintString(const char* aString)
{
   mPrintFlag = false;
   my_strncpy(mString,aString,cMaxStringSize);
}

PrintString::PrintString(const PrintString& aRhs)
{
   mPrintFlag = aRhs.mPrintFlag;
   my_strncpy(mString, aRhs.mString, cMaxStringSize);
}
PrintString& PrintString::operator= (const PrintString& aRhs)
{
   mPrintFlag = aRhs.mPrintFlag;
   my_strncpy(mString, aRhs.mString, cMaxStringSize);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Copy an input string to the member string.

void PrintString::puts(const char* aString)
{
   // Copy the input string.
   my_strncpy(mString, aString, cMaxStringSize);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send this instance to the print thread. The print thread will
// eventually delete this instance. Do not use this instance after
// sending it. If this is not successful then it deletes itself.

void PrintString::sendToPrintThread()
{
   // Guard.
   bool tPass = true;
   if (gPrintThread == 0)         tPass = false;
   if (!gShare.mPrintEnableFlag)  tPass = false;

   // Try to write to the print thread queue.
   if (tPass) tPass = gPrintThread->tryWriteString(this);

   // If not successful then delete this instance.
   if (!tPass) delete this;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

