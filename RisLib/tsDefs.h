#pragma once

/*==============================================================================
Thread Services.
These provide printing, logging, and error handling services that are
based on having the controlling parameters located in thread local storage.
This allows the services to be controlled on an individual thread basis.

Definitions.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{
   class CmdLineCmd;
}

namespace TS
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a struct that encapsulates print levels for thread services 
// prints to stdout and the log file.

typedef struct PrintLevel
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Print levels for stdout and log file.
   short mOutLevel;
   short mLogLevel;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   PrintLevel();
   PrintLevel(short aOutLevel,short aLogLevel);

   // Convert to string for prints.
   char* asString(char* aBuffer);

   // Read command line command arguments.
   void readArgs(Ris::CmdLineCmd* aCmd);

} PrintLevel;

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

