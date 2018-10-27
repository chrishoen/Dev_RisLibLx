#include "stdafx.h"

#include "risCmdLineConsole.h"
#include "CmdLineExec.h"
#include "MainInit.h"

#include "someMasterThread.h"
#include "someSlaveThread.h"
#include "someRandomTimerThread.h"

using namespace Some;

//******************************************************************************
//******************************************************************************
//******************************************************************************

int main(int argc,char** argv)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize program.

   main_initialize(argc,argv);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Launch program threads.

   Some::gSlaveThread = new Some::SlaveThread;
   Some::gSlaveThread->launchThread();

   Some::gMasterThread = new Some::MasterThread;
   Some::gMasterThread->launchThreads();

   Some::gRandomTimerThread1 = new Some::RandomTimerThread(1);
   Some::gRandomTimerThread1->launchThread();

   Some::gRandomTimerThread2 = new Some::RandomTimerThread(2);
   Some::gRandomTimerThread2->launchThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Start user command line executive, wait for user to exit.

   CmdLineExec* exec = new CmdLineExec;
   Ris::executeCmdLineConsole(exec);
   delete exec;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Shutdown program Threads.

   Some::gRandomTimerThread1->shutdownThread();
   Some::gRandomTimerThread2->shutdownThread();
   Some::gMasterThread->shutdownThreads();
   Some::gSlaveThread->shutdownThread();

   delete Some::gRandomTimerThread1;
   delete Some::gRandomTimerThread2;
   delete Some::gMasterThread;
   delete Some::gSlaveThread;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Finalize program.

   main_finalize();

   printf("press enter\n");
   getchar();

   return 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
