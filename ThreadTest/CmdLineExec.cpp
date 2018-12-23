
#include "stdafx.h"

#include "someRandomTimerThread.h"
#include "someMasterThread.h"
#include "someSlaveThread.h"
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

void CmdLineExec::executeTP(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, false);

   Some::gRandomTimerThread1->mTPFlag = aCmd->argBool(1);
   Some::gRandomTimerThread2->mTPFlag = aCmd->argBool(1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, 101);

   Some::gMasterThread->mTest1QCall(7, aCmd->argInt(1));
   return;

   Some::gMasterThread->mTest0QCall();
   return;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, 101);
   Some::gSlaveThread->mTest1QCall(6, aCmd->argInt(1));
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

