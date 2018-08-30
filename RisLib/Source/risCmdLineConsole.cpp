/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"


#include "risPortableCalls.h"
#include "risThreadsThreads.h"
#include "my_functions.h"
#include "prnPrint.h"

#define  _RISCMDLINECONSOLE_CPP_
#include "risCmdLineConsole.h"


namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// This enters a loop that reads console input and applies a command line 
// executive to each line that is entered by a user. The loop terminates when
// the user exits.

void CmdLineConsole::execute (BaseCmdLineExec* aExec)
{            
   // Locals
   char tCommandLine[200];
   CmdLineCmd  tCmd;

   // Reset the executive
   aExec->reset();

   while(true)
   {
      // Read console input
	   if (fgets(tCommandLine, 200, stdin) == 0)
	   {
		   return;
	   }

      // Remove cr/lf at end of line
      my_trimCRLF(tCommandLine);

      // Test for toggle suppress print
      if (strcmp(tCommandLine,"p")==0)
      {
         Prn::toggleSuppressPrint();
      }
      // Process command line 
      else
      {
         // Special case, nickname
         // Exit
         if (strcmp(tCommandLine,"e")==0)
         {
            return;
         }

         // Parse buffer to get command line command
         tCmd.parseCmdLine(tCommandLine);

         // Test for valid command line command
         if (tCmd.mValidFlag)
         {
            // Test for exit
            if(tCmd.isCmd("EXIT"))
            {
               return;; 
            }
            // Not exit
            else
            {
               // execute the command line command with the given executive
               aExec->execute(&tCmd);

               // Test for bad command
               // This is true if the executive didn't accept the command
               if(tCmd.isBadCmd()) printf("INVALID COMMAND\n");
            }
         }
         // Invalid command line command
         else
         {
            printf("INVALID COMMAND\n");
         }
      }
   }
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

