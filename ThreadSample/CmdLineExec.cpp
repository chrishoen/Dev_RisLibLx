#include "stdafx.h"


#include "CmdLineExec.h"

#include "someQCallThread1.h"
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
   if(aCmd->isCmd("GO1"   ))  executeGo1     (aCmd);
   if(aCmd->isCmd("GO2"   ))  executeGo2     (aCmd);
   if(aCmd->isCmd("GO3"   ))  executeGo3     (aCmd);
   if(aCmd->isCmd("C101"  ))  executeC101    (aCmd);
   if(aCmd->isCmd("C102"  ))  executeC102    (aCmd);
}

//******************************************************************************

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   gQCallThread1->mC102QCall(1001);
}

//******************************************************************************

void CmdLineExec::executeGo3(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************

void CmdLineExec::executeC101(Ris::CmdLineCmd* aCmd)
{
   gQCallThread1->mC101QCall(101);
}

//******************************************************************************

void CmdLineExec::executeC102(Ris::CmdLineCmd* aCmd)
{
   gQCallThread1->mC102QCall(1001);
}

