#include "stdafx.h"

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
   if(aCmd->isCmd("RESET"  ))  reset();
   if(aCmd->isCmd("GO1"    ))  executeGo1(aCmd);
   if(aCmd->isCmd("GO2"    ))  executeGo2(aCmd);
   if(aCmd->isCmd("GO3"    ))  executeGo3(aCmd);
   if(aCmd->isCmd("GO4"    ))  executeGo4(aCmd);
   if(aCmd->isCmd("GO5"    ))  executeGo5(aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

//******************************************************************************
//******************************************************************************
//******************************************************************************

#define POLLIN 0x001

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, 101);
   aCmd->setArgDefault(2, 202.2);
   aCmd->setArgDefault(3, "abcd");

   char tWhole[100];
   aCmd->copyArgWhole(tWhole, 100);

   Prn::print(0, "arg1     %-10d",   aCmd->argInt(1));
   Prn::print(0, "arg2     %-10.2f", aCmd->argDouble(2));
   Prn::print(0, "arg3     %-10s",   aCmd->argString(3));
   Prn::print(0, "argw1    %-20s",   aCmd->argWhole());
   Prn::print(0, "argw2    %-20s",   tWhole);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   char tWhole[100];
   aCmd->copyArgWhole(tWhole, 100);

   Prn::print(0, "arg1     %-10d", aCmd->argInt(1));
   Prn::print(0, "arg2     %-10.2f", aCmd->argDouble(2));
   Prn::print(0, "arg3     %-10s", aCmd->argString(3));
   Prn::print(0, "argw1    %-20s", aCmd->argWhole());
   Prn::print(0, "argw2    %-20s", tWhole);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo3(Ris::CmdLineCmd* aCmd)
{
   char tString[200];

   while (true)
   {
      fgets(tString, 200, stdin);
      printf("CMD %d %s", (int)strlen(tString), tString);
      if (strcmp(tString, "e\n") == 0) break;
   }

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

