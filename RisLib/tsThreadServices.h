#pragma once

/*==============================================================================
Thread Services.
These provide printing, logging, and error handling services that are
based on having the controlling parameters located in thread local storage.
This allows the services to be controlled on an individual thread basis.

The API.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "tsThreadLocal.h"

namespace TS
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize the thread services.

   void reset ();
   void setProgramName(const char* aName);
   void setProgramLogFilepath(const char* aFilepath);
   void setProgramPrintLevel(PrintLevel aPrintLevel);
   void initialize();
   void finalize();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Access to thread local storage.

   // Set the pointer to the thread local storage. This should be called
   // at the begining of the thread run function.
   void setThreadLocal(ThreadLocal* aLocal);

   // Return a pointer to the thread local storage.
   ThreadLocal* tls();

   // Return true if the thread local storage is initialized.
   bool isEnabled();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Prints that are controlled by the thread local storage variables.

   // Print. This is input a print level 0,1,2 and the signature for a
   // printf. If the input level is less than or equal to the print level
   // that is located in thread local storage then the print is performed
   // else it is ignored. If the print is performed it prints a string
   // to stdout and and to the log file.
   void print(int aLevel, const char* aFormat, ...);


//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

