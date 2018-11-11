#pragma once

/*==============================================================================
Thread Services.
These provide printing, logging, and error handling services that are
based on having the controlling parameters located in thread local storage.

Thread local storage class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risByteContent.h"
#include "risByteMsgMonkey.h"
#include "risThreadsQCall.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace TS
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// For each thread, an instance of this class is created and its pointer
// is stored as a thread local variable.

class ThreadLocal
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants.

   static const int cMaxStringSize = 128;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // A test code.
   int mCode;

   // Thread name.
   char mThreadName[cMaxStringSize];

   // The print and log levels for the thread. 
   int mPrintLevel;
   int mLogLevel;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   ThreadLocal();

   // Set member.
   void setThreadName (const char* aName);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

