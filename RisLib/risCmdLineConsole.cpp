/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"


#include "risPortableCalls.h"
#include "risThreadsThreads.h"
#include "my_functions.h"
#include "tsThreadServices.h"
#include "prnPrint.h"
#include "conInput.h"

#define  _RISCMDLINECONSOLE_CPP_
#include "risCmdLineConsole.h"


namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This enters a loop that reads console input and applies a command line 
// executive to each line that is entered by a user. The loop terminates when
// the user exits.

void CmdLineConsole::execute (BaseCmdLineExec* aExec)
{            
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize console input.

   TS::print(1, "");
   TS::print(1, "Command Line Executive BEGIN");

   // Initialize console input.
   Con::initializeInput();

   // Reset the executive.
   aExec->reset();

   // Locals.
   char tCommandLine[Con::cMaxStringSize];
   CmdLineCmd  tCmd;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Loop for console input.

   while(true)
   {
      // Read console input.
      Con::doReadInputString(tCommandLine);

      //************************************************************************
      //************************************************************************
      //************************************************************************
      // Test for specific commands.

      if (strlen(tCommandLine)==0)
      {
         printf("\n");
      }
      else if (strcmp(tCommandLine, "enter") == 0)
      {
         printf("\n");
      }
      else if (strcmp(tCommandLine, "escape") == 0)
      {
         break;
      }
      else if (strcmp(tCommandLine, "alt_p") == 0)
      {
         Prn::unsuppressPrint();
      }
      else if (strcmp(tCommandLine, "alt_o") == 0)
      {
         Prn::suppressPrint();
      }

      //************************************************************************
      //************************************************************************
      //************************************************************************
      // Process command line.

      else
      {
         // Parse command line input string to get command line command.
         tCmd.parseCmdLine(tCommandLine);

         // Test for valid command line command.
         if (tCmd.mValidFlag)
         {
            // Test for exit.
            if(tCmd.isCmd("EXIT"))
            {
               break;
            }
            // Not exit
            else
            {
               // Execute the command line command with the given executive.
               aExec->execute(&tCmd);

               // Test for bad command. This is true if the executive didn't
               // accept the command.
               if(tCmd.isBadCmd()) printf("INVALID COMMAND\n");
            }
         }
         // Invalid command line command.
         else
         {
            printf("INVALID COMMAND\n");
         }
      }
   }

   // Done.
   Con::finalizeInput();
   TS::print(1, "Command Line Executive END");
   return;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Nickname.

void executeCmdLineConsole (BaseCmdLineExec* aExec)
{
   gCmdLineConsole.execute(aExec);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Console helper.

void setConsoleTitle(char* aTitle)
{
   portableSetConsoleTitle(aTitle);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Console thread:

class ConsoleThread : public Ris::Threads::BaseThread
{
public:
   BaseCmdLineExec* mExec;

	ConsoleThread(BaseCmdLineExec* aExec)
	{
		mExec=aExec;
	}

	void threadRunFunction()
	{
		gCmdLineConsole.execute(mExec);
	}

	void shutdownThread()
	{
		forceTerminateThread();
	}
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

ConsoleThread* gConsoleThread=0;

//******************************************************************************
//******************************************************************************
//******************************************************************************

void executeCmdLineConsoleThread(BaseCmdLineExec* aExec)
{
	gConsoleThread = new ConsoleThread(aExec);
	gConsoleThread->launchThread();
	gConsoleThread->waitForThreadTerminate();
	delete gConsoleThread;
	gConsoleThread=0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void abortCmdLineConsole ()
{
	if (gConsoleThread)
	{
		gConsoleThread->shutdownThread();
	}
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

