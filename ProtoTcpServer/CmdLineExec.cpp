
#include "stdafx.h"

#include "procoTcpSettings.h"
#include "procoMsg.h"
#include "procoMsgHelper.h"

#include "procoServerThread.h"

#include "CmdLineExec.h"

using namespace ProtoComm;

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
   if (aCmd->isCmd("TP"))        gServerThread->mTPFlag = aCmd->argBool(1);
   if (aCmd->isCmd("TX"))        executeTx(aCmd);
   if (aCmd->isCmd("ECHO"))      executeEcho(aCmd);
   if (aCmd->isCmd("DATA"))      executeData(aCmd);
   if (aCmd->isCmd("GO1"))       executeGo1(aCmd);
   if (aCmd->isCmd("GO2"))       executeGo2(aCmd);
   if (aCmd->isCmd("T1"))        executeTest1(aCmd);
   if (aCmd->isCmd("Parms"))     executeParms(aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeTx (Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,201);

   gServerThread->sendTestMsg(aCmd->argInt(1));
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeEcho(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, 0);
   aCmd->setArgDefault(2, 0);
   int tSessionIndex = aCmd->argInt(1);
   int tNumWords = aCmd->argInt(2);

   ProtoComm::EchoRequestMsg* tMsg = new ProtoComm::EchoRequestMsg;
   MsgHelper::initialize(tMsg, tNumWords);
   gServerThread->sendMsg(tSessionIndex, tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeData(Ris::CmdLineCmd* aCmd)
{
   ProtoComm::DataMsg* tMsg = new ProtoComm::DataMsg;
   MsgHelper::initialize(tMsg);

   gServerThread->sendMsg(0,tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,1);

   ProtoComm::TestMsg* tMsg = new ProtoComm::TestMsg;
 
   gServerThread->sendMsg(0,tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,1);
   int tN = aCmd->argInt(1);

   for (int i=0;i<tN;i++)
   {
      ProtoComm::EchoRequestMsg* tMsg = new ProtoComm::EchoRequestMsg;
 
      gServerThread->sendMsg(0,tMsg);
      gServerThread->threadSleep(10);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeTest1(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeParms(Ris::CmdLineCmd* aCmd)
{
   ProtoComm::gTcpSettings.show();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
