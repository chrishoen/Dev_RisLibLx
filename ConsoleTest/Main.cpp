#include "stdafx.h"

#include "MainInit.h"
#include "risCmdLineConsole.h"
#include "CmdLineExec.h"

void amain_init();


//******************************************************************************
int main(int argc,char** argv)
{
   //--------------------------------------------------------------------
   // Begin program.

   main_initialize(argc,argv);

   //--------------------------------------------------------------------
   // Start user command line executive,
   // Wait for user to exit.

   CmdLineExec* tExec = new CmdLineExec;
	Ris::executeCmdLineConsoleThread(tExec);
   delete tExec;

   //--------------------------------------------------------------------
   // End program.

   main_finalize();
   return 0;
}
