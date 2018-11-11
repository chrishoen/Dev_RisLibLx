#include "stdafx.h"

#include "risThreadsProcess.h"
#include "risCmdLineConsole.h"
#include "CmdLineExec.h"

#include "someRandomTimerThread.h"

#include "MainInit.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

int main(int argc,char** argv)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Begin program.

   main_initialize(argc,argv);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Launch program threads.

   if (true)
   {
      Some::gRandomTimerThread1 = new Some::RandomTimerThread(1);
      Some::gRandomTimerThread1->launchThread();
   }

   if (true)
   {
      Some::gRandomTimerThread2 = new Some::RandomTimerThread(2);
      Some::gRandomTimerThread2->launchThread();
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Show program threads.

   Ris::Threads::showCurrentThreadInfo();
   if (Some::gRandomTimerThread1) Some::gRandomTimerThread1->showThreadInfo();
   if (Some::gRandomTimerThread2) Some::gRandomTimerThread2->showThreadInfo();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Execute user command line executive, wait for user to exit.

   CmdLineExec* tExec = new CmdLineExec;
   Ris::gCmdLineConsole.execute(tExec);
   delete tExec;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Shutdown program threads.

   if (Some::gRandomTimerThread1)
   {
      Some::gRandomTimerThread1->shutdownThread();
      delete Some::gRandomTimerThread1;
      Some::gRandomTimerThread1 = 0;
   }

   if (Some::gRandomTimerThread2)
   {
      Some::gRandomTimerThread2->shutdownThread();
      delete Some::gRandomTimerThread2;
      Some::gRandomTimerThread2 = 0;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // End program.

   main_finalize();
   return 0;

   printf("press enter\n");
   getchar();
   return 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

