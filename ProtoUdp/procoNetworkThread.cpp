/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "risSockets.h"
#include "procoUdpSettings.h"
#include "procoMsgHelper.h"

#define  _PROCONETWORKTHREAD_CPP_
#include "procoNetworkThread.h"

namespace ProtoComm
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

NetworkThread::NetworkThread()
{
   // Set base class thread services.
   BaseClass::setThreadName("Network");
   BaseClass::setThreadPriorityHigh();
   BaseClass::setThreadPrintLevel(TS::PrintLevel(3, 3));

   // Set base class thread priority.
   BaseClass::setThreadPriorityHigh();

   // Set base class timer period.
   BaseClass::mTimerPeriod = gUdpSettings.mThreadTimerPeriod;

   // Initialize qcalls.
   mRxMsgQCall.bind(this, &NetworkThread::executeRxMsg);

   // Initialize variables.
   mUdpMsgThread = 0;
   mMonkeyCreator.configure(gUdpSettings.mMyAppNumber);
   mTPFlag = false;
   mStatusCount1=0;
   mStatusCount2=0;
}

NetworkThread::~NetworkThread()
{
   delete mUdpMsgThread;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immediately 
// after the thread starts running. It starts the child thread.

void NetworkThread::threadInitFunction()
{
   // Instance of network socket settings.
   Ris::Net::Settings tSettings;

   tSettings.setLocalIp  (gUdpSettings.mMyUdpIPAddress,    gUdpSettings.mMyUdpPort);
   tSettings.setRemoteIp (gUdpSettings.mOtherUdpIPAddress, gUdpSettings.mOtherUdpPort);
   tSettings.mMonkeyCreator = &mMonkeyCreator;
   tSettings.mRxMsgQCall = mRxMsgQCall;
   tSettings.mPrintLevel = gUdpSettings.mPrintLevel;

   // Create the child thread with the settings.
   mUdpMsgThread = new Ris::Net::UdpMsgThread(tSettings);

   // Launch the child thread.
   mUdpMsgThread->launchThread();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function, base class overload.

void NetworkThread::threadExitFunction()
{
   // Shutdown the child thread.
   mUdpMsgThread->shutdownThread();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show thread state info, base class overload.

void NetworkThread::showThreadInfo()
{
   BaseClass::showThreadInfo();
   if (mUdpMsgThread) mUdpMsgThread->showThreadInfo();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// QCall registered to the mUdpMsgThread child thread. It is invoked when
// a message is received. It process the received messages.

void NetworkThread::executeRxMsg(Ris::ByteContent* aMsg)
{
   ProtoComm::BaseMsg* tMsg = (ProtoComm::BaseMsg*)aMsg;

   // Message jump table based on message type.
   // Calls corresponding specfic message handler method.
   switch (tMsg->mMessageType)
   {
      case ProtoComm::MsgIdT::cTestMsg :
         processRxMsg((ProtoComm::TestMsg*)tMsg);
         break;
      case ProtoComm::MsgIdT::cEchoRequestMsg :
         processRxMsg((ProtoComm::EchoRequestMsg*)tMsg);
         break;
      case ProtoComm::MsgIdT::cEchoResponseMsg :
         processRxMsg((ProtoComm::EchoResponseMsg*)tMsg);
         break;
      case ProtoComm::MsgIdT::cDataMsg :
         processRxMsg((ProtoComm::DataMsg*)tMsg);
         break;
      default :
         Prn::print(Prn::ThreadRun1, "NetworkThread::executeServerRxMsg ??? %d",tMsg->mMessageType);
         delete tMsg;
         break;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - TestMsg.

void NetworkThread::processRxMsg(TestMsg* aMsg)
{
   Prn::print(Prn::ThreadRun1, "processRxMsg_TestMsg");
   delete aMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - EchoRequestMsg.

void NetworkThread::processRxMsg(EchoRequestMsg* aMsg)
{
   Prn::print(Prn::ThreadRun1, "processRxMsg_EchoRequestMsg %d %d", aMsg->mCode1, aMsg->mNumWords);

   EchoResponseMsg* tTxMsg = new EchoResponseMsg;
   tTxMsg->mCode1 = aMsg->mCode1;
   tTxMsg->mNumWords = aMsg->mNumWords;
   sendMsg(tTxMsg);

   delete aMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - EchoResponseMsg.

void NetworkThread::processRxMsg(EchoResponseMsg* aMsg)
{
   Prn::print(Prn::ThreadRun1, "processRxMsg_EchoResponseMsg %d %d", aMsg->mCode1, aMsg->mNumWords);
   delete aMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - DataMsg.

void NetworkThread::processRxMsg(DataMsg* aMsg)
{
   Prn::print(Prn::ThreadRun1, "processRxMsg_DataMsg");
   MsgHelper::show(aMsg);
   delete aMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a message via the child socket thread.

void NetworkThread::sendMsg (ProtoComm::BaseMsg* aMsg)
{
   mUdpMsgThread->sendMsg(aMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a message via the child socket thread.

void NetworkThread::sendTestMsg()
{
   ProtoComm::TestMsg* tMsg = new ProtoComm::TestMsg;
 
   mUdpMsgThread->sendMsg(tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Execute periodically. This is called by the base class timer. It
// sends an echo request message.

void NetworkThread::executeOnTimer(int aTimerCount)
{
   if (!mTPFlag) return;
   Prn::print(Prn::ThreadRun3, "NetworkThread::executeOnTimer %d", aTimerCount);

   // Send a status request message.
   EchoRequestMsg* tMsg = new EchoRequestMsg;
   tMsg->mCode1 = aTimerCount;
   tMsg->mNumWords = gUdpSettings.mNumWords;
   sendMsg(tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace