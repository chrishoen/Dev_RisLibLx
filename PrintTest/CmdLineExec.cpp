
#include "stdafx.h"

#include "CmdLineExec.h"

#include "risNetPortDef.h"
#include "risNetUdpStringSocket.h"
#include "risNetSettings.h"

#include "someTimerThread.h"

#include "prnPrint.h"
#include "prnPrintSettings.h"

using namespace Some;

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

void CmdLineExec::execute(Ris::CmdLineCmd* aCmd)
{
   if(aCmd->isCmd("TP"    ))  gTimerThread->mTPFlag = aCmd->argBool(1);
   if(aCmd->isCmd("GO1"   ))  executeGo1     (aCmd);
   if(aCmd->isCmd("GO2"   ))  executeGo2     (aCmd);
   if(aCmd->isCmd("GO3"   ))  executeGo3     (aCmd);
   if(aCmd->isCmd("GO4"   ))  executeGo4     (aCmd);
   if(aCmd->isCmd("GO5"   ))  executeGo5     (aCmd);
   if(aCmd->isCmd("PARMS" ))  executeParms   (aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
   Prn::print(0,"Go1 0");
   Prn::print(Prn::ProcRun1,"Go1 Prn::ProcRun1");
   Prn::print(Prn::ProcRun2,"Go1 Prn::ProcRun2");
   Prn::print(Prn::ProcRun3,"Go1 Prn::ProcRun3");
   Prn::print(Prn::ProcRun4,"Go1 Prn::ProcRun4");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   Prn::print(0,"Go2");
   Log::write(0,"Go2");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo3(Ris::CmdLineCmd* aCmd)
{
   Prn::print(Prn::ViewRun3,"Go3 Prn::ViewRun3");
   Prn::print(Prn::ViewRun4,"Go3 Prn::ViewRun4");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo4(Ris::CmdLineCmd* aCmd)
{
   Prn::print(0,"Go4 ZERO");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo5(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, 1);
   int tConsole = aCmd->argInt(1);

   Ris::Net::UdpTxStringSocket tSocket;

   Ris::Net::Settings tSettings;
   tSettings.setRemoteIp(
      Prn::gPrintSettings.mPrintViewHostIPAddress,
      Prn::gPrintSettings.mPrintViewHostIPPort + tConsole);
   tSocket.initialize(tSettings);
   tSocket.configure();

   tSocket.doSendString("ABCDEFG");
   tSocket.doClose();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeParms(Ris::CmdLineCmd* aCmd)
{
   Prn::gPrintSettings.reset();
   Prn::gPrintSettings.readSection("default");
   Prn::gPrintSettings.show();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************


