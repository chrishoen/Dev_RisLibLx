#include "stdafx.h"

#include "risCmdLineConsole.h"
#include "CmdLineExec.h"
#include "MainInit.h"

#include "someTimerThread.h"
using namespace Some;

//******************************************************************************
int main(int argc,char** argv)
{
   //--------------------------------------------------------------------
   // Initialize

   main_initialize(argc,argv);

   //--------------------------------------------------------------------
   // Launch threads

   gTimerThread = new TimerThread;
   gTimerThread->launchThread();

   //--------------------------------------------------------------------
   // Start user command line executive,
   // It returns when user exits

   CmdLineExec* tExec = new CmdLineExec;
   Ris::executeCmdLineConsole(tExec);
   delete tExec;

   //--------------------------------------------------------------------
   // Shutdown threads

   gTimerThread->shutdownThread();
   delete gTimerThread;

   //--------------------------------------------------------------------
   // Exit
   
   main_finalize();

   return 0;
}
