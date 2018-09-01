#include "stdafx.h"

#include <unistd.h>

#include "CmdLineExec.h"
#include "Parms.h"
#include "someMyClass.h"
#include "risAlphaDir.h"

using namespace Some;

//******************************************************************************
CmdLineExec::CmdLineExec()
{
}
//******************************************************************************
void CmdLineExec::reset()
{
}

//******************************************************************************
void CmdLineExec::execute(Ris::CmdLineCmd* aCmd)
{
   if (aCmd->isCmd("RESET"))  reset();
   if (aCmd->isCmd("GO1"))    executeGo1(aCmd);
   if (aCmd->isCmd("GO2"))    executeGo2(aCmd);
   if (aCmd->isCmd("GO3"))    executeGo3(aCmd);
   if (aCmd->isCmd("GO4"))    executeGo4(aCmd);
   if (aCmd->isCmd("GO5"))    executeGo5(aCmd);
   if (aCmd->isCmd("GO6"))    executeGo6(aCmd);
   if (aCmd->isCmd("Parms"))  executeParms(aCmd);
   if (aCmd->isCmd("Parms2")) executeParms2(aCmd);

   if (aCmd->isCmd("RUN1"))  executeRun1(aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
   char tBuffer[400];

   Prn::print(0, "AlphaDir         %s", Ris::getAlphaDirectory());
   Prn::print(0, "Alpha Bin        %s", Ris::getAlphaFilePath_Work(tBuffer));
   Prn::print(0, "Alpha Bin        %s", Ris::getAlphaFilePath_Work(tBuffer, "BackEnd.cs"));

}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   Prn::print(0, "portableGetCurrentDir         %s", Ris::portableGetCurrentDir());
   Prn::print(0, "portableGetProgramDir         %s", Ris::portableGetProgramDir());
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo3(Ris::CmdLineCmd* aCmd)
{
   char tBuffer[400];
   readlink("/proc/self/exe", tBuffer, 400);
   Prn::print(0, "/proc/self/exe  %s", tBuffer);

   bool tGoing = true;
   int tIndex = strlen(tBuffer)-1;
   while (tGoing)
   {
      if (tBuffer[tIndex] == '/')
      {
         tBuffer[tIndex+1] = 0;
         tGoing = false;
      }
      if (--tIndex == 0) tGoing = false;
   }

   Prn::print(0, "tBuffer         %s", tBuffer);
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

void my_local_function()
{
   printf("my_local_function\n");
}

void CmdLineExec::executeGo6(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeRun1(Ris::CmdLineCmd* aCmd)
{
   gParms.reset();
   gParms.readSection("default");
   gParms.readSection("run1");

   MyClass tMyClass(gParms.mMyClassParms);
   tMyClass.doRun1();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeParms(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,"Run1");
   gParms.reset();
   gParms.readSection("default");
   gParms.readSection(aCmd->argString(1));
   gParms.show();
}


//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeParms2(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,"Run1");
   gParms.setExplicitFileDir("~/Alpha/Work");
   gParms.reset();
   gParms.readSection("default");
   gParms.readSection(aCmd->argString(1));
   gParms.show();
}


