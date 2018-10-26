
#include "stdafx.h"

#include "procoUdpSettings.h"
#include "procoMsg.h"
#include "procoMsgHelper.h"

#include "procoNetworkThread.h"

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
   if (aCmd->isCmd("TP"))        ProtoComm::gNetworkThread->mTPFlag = aCmd->argBool(1);
   if (aCmd->isCmd("TX"))        executeTx(aCmd);
   if (aCmd->isCmd("ECHO"))      executeEcho (aCmd);
   if (aCmd->isCmd("DATA"))      executeData(aCmd);
   if (aCmd->isCmd("GO1"))       executeGo1  (aCmd);
   if (aCmd->isCmd("GO2"))       executeGo2  (aCmd);
   if (aCmd->isCmd("Parms"))     executeParms(aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeTx (Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,1);
   int tMsgType= aCmd->argInt(1);

   switch (tMsgType)
   {
      case 1:
      {
         ProtoComm::TestMsg* tMsg = new ProtoComm::TestMsg;
         MsgHelper::initialize(tMsg);
         gNetworkThread->sendMsg(tMsg);
         break;
      }
      case 5:
      {
         ProtoComm::DataMsg* tMsg = new ProtoComm::DataMsg;
         MsgHelper::initialize(tMsg);
         gNetworkThread->sendMsg(tMsg);
         break;
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeEcho(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, 0);
   int tNumWords = aCmd->argInt(1);
   
   ProtoComm::EchoRequestMsg* tMsg = new ProtoComm::EchoRequestMsg;
   MsgHelper::initialize(tMsg,tNumWords);
   gNetworkThread->sendMsg(tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeData(Ris::CmdLineCmd* aCmd)
{
   ProtoComm::DataMsg* tMsg = new ProtoComm::DataMsg;
   MsgHelper::initialize(tMsg);

   gNetworkThread->sendMsg(tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo1 (Ris::CmdLineCmd* aCmd)
{
   gNetworkThread->sendTestMsg();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   ProtoComm::EchoRequestMsg* tMsg = new ProtoComm::EchoRequestMsg;

   gNetworkThread->sendMsg(tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeParms(Ris::CmdLineCmd* aCmd)
{
   ProtoComm::gUdpSettings.show();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

