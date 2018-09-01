#pragma once
/*==============================================================================
Console command line processor
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risCmdLineExec.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This enters a loop that reads console input and applies a command line 
// executive to each line that is entered by a user. The loop terminates when
// the user exits.

class CmdLineConsole
{
public:
   // Execute command line executive from console input.
   // Returns when user exits.
   void execute (BaseCmdLineExec* aExec);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Helpers:

// Execute in the context of the calling thread.
void  executeCmdLineConsole (BaseCmdLineExec* aExec);

// Execute in the context of a separate thread.
void  executeCmdLineConsoleThread (BaseCmdLineExec* aExec);

// Abort, same as entering "Exit"
void  abortCmdLineConsole ();

// Set the console title.
void  setConsoleTitle(char* aTitle);

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global singular instance.

#ifdef _RISCMDLINECONSOLE_CPP_
       CmdLineConsole  gCmdLineConsole;
#else
       extern CmdLineConsole  gCmdLineConsole;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

