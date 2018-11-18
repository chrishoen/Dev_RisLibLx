#pragma once
/*==============================================================================
Thread Services.
These provide printing, logging, and error handling services that are
based on having the controlling parameters located in thread local storage.

print string class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace TS
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This class encalsulates a string that can be printed and logged by
// the thread services print function.

class PrintString
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants.

   static const int cMaxStringSize = 200;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // If true then print the string to stdout.
   bool mOutFlag;

   // Null terminated string.
   char mString[cMaxStringSize];

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   PrintString();
   PrintString(const char* aString);

   // Copy constructor and assignment operator.
   PrintString(const PrintString& aRhs);
   PrintString& operator= (const PrintString& aRhs);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Copy an input string to the member string.
   void puts(const char* aString);

   // Send this instance to the print thread. The print thread will
   // eventually delete this instance. Do not use this instance after
   // sending it. If this is not successful then it deletes itself.
   void sendToPrintThread();

};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace




