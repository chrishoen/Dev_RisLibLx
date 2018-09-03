#include "stdafx.h"

#include "risCmdLineConsole.h"
#include "CmdLineExec.h"

#include "someQCallThread1.h"
#include "someTimerThread1.h"
#include "GSettings.h"
#include "MainInit.h"

using namespace Some;

//******************************************************************************
int main(int argc,char** argv)
{
   //--------------------------------------------------------------------
   // Initialize

   main_initialize(argc,argv);

   //--------------------------------------------------------------------
   // Launch threads

   if (gGSettings.mTestThread == GSettings::cTestThread_QCallThread1)
   {
      gQCallThread1 = new QCallThread1;
      gQCallThread1->launchThread();
   }

   if (gGSettings.mTimerThread == GSettings::cTimerThread_Thread1)
   {
      gTimerThread1 = new TimerThread1;
      gTimerThread1->launchThread();
   }

   //--------------------------------------------------------------------
   // Start user command line executive,
   // It returns when user exits

   CmdLineExec* tExec = new CmdLineExec;
   Ris::executeCmdLineConsole(tExec);
   delete tExec;

   //--------------------------------------------------------------------
   // Shutdown threads

   if (gGSettings.mTimerThread == GSettings::cTimerThread_Thread1)
   {
      gTimerThread1->shutdownThread();
      delete gTimerThread1;
   }

   if (gGSettings.mTestThread == GSettings::cTestThread_QCallThread1)
   {
      gQCallThread1->shutdownThread();
      delete gQCallThread1;
   }

   //--------------------------------------------------------------------
   // Exit
   
   main_finalize();

   return 0;
}

