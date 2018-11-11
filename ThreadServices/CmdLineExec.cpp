
#include "stdafx.h"

#include "someRandomTimerThread.h"
#include "someThreadParms.h"

#include "risCmdLineConsole.h"
#include "CmdLineExec.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

CmdLineExec::CmdLineExec()
{
}

void CmdLineExec::reset()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This class is the program command line executive. It processes user
// command line inputs and executes them. It inherits from the command line
// command executive base class, which provides an interface for executing
// command line commands. It provides an override execute function that is
// called by a console executive when it receives a console command line input.
// The execute function then executes the command.

void CmdLineExec::execute(Ris::CmdLineCmd* aCmd)
{
   if (aCmd->isCmd("TP1"))       Some::gRandomTimerThread1->mTPFlag = aCmd->argBool(1);
   if (aCmd->isCmd("TP2"))       Some::gRandomTimerThread2->mTPFlag = aCmd->argBool(1);
   if (aCmd->isCmd("TP"))        executeTP(aCmd);
   if (aCmd->isCmd("GO1"))       executeGo1(aCmd);
   if (aCmd->isCmd("GO2"))       executeGo2(aCmd);
   if (aCmd->isCmd("GO3"))       executeGo3(aCmd);
   if (aCmd->isCmd("GO4"))       executeGo4(aCmd);
   if (aCmd->isCmd("GO5"))       executeGo5(aCmd);
   if (aCmd->isCmd("GO5"))       executeGo5(aCmd);
   if (aCmd->isCmd("Parms"))     executeParms(aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
// aCmd->setArgDefault(1, "0123");
   char tSource[100];
   char tDestin[100];
   int tSize = 4;
   aCmd->copyArgString(1, tSource, 100);

   my_strncpy(tDestin, tSource, tSize);

   Prn::print(0, "source %3d %s", strlen(tSource), tSource);
   Prn::print(0, "destin %3d %s", strlen(tDestin), tDestin);

}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeTP(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, false);

   if (Some::gRandomTimerThread1)
   {
      Some::gRandomTimerThread1->mTPFlag = aCmd->argBool(1);
   }
   if (Some::gRandomTimerThread2)
   {
      Some::gRandomTimerThread2->mTPFlag = aCmd->argBool(1);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   // Set defaults if no arguments were entered.
   aCmd->setArgDefault(1,"something");

   // Show arguments.
   Prn::print(0,"Go2 %s %10.6f",aCmd->argString(1));
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo3(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo4(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo5(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeParms(Ris::CmdLineCmd* aCmd)
{
   Some::gThreadParms.reset();
   Some::gThreadParms.readSection("default");
   Some::gThreadParms.show();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

