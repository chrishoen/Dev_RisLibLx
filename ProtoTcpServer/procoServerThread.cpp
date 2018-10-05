/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "procoMsgHelper.h"
#include "procoTcpSettings.h"

#define  _PROCOSERVERTHREAD_CPP_
#include "procoServerThread.h"

namespace ProtoComm
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

ServerThread::ServerThread()
{
   // Set base class thread priority.
   BaseClass::setThreadPriorityHigh();

   // Set base class timer period.
   BaseClass::mTimerPeriod = gTcpSettings.mThreadTimerPeriod;

   // Initialize qcalls.
   mSessionQCall.bind(this, &ServerThread::executeSession);
   mRxMsgQCall.bind(this,   &ServerThread::executeRxMsg);

   // Initialize variables.
   mTcpMsgServerThread = 0;
   mMonkeyCreator.configure(gTcpSettings.mMyAppNumber);
   mTPFlag = false;
   mStatusCount1 = 0;
   mStatusCount2 = 0;
}

ServerThread::~ServerThread()
{
   delete mTcpMsgServerThread;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immediately 
// after the thread starts running. It starts the child thread.

void ServerThread::threadInitFunction()
{
   Prn::print(Prn::ThreadInit1, "ServerThread::threadInitFunction");

   // Instance of network socket settings.
   Ris::Net::Settings tSettings;

   tSettings.setLocalIp(gTcpSettings.mTcpServerIPAddress, gTcpSettings.mTcpServerPort);
   tSettings.mMonkeyCreator = &mMonkeyCreator;
   tSettings.mMaxSessions = gTcpSettings.mTcpMaxSessions;
   tSettings.mServerSessionQCall = mSessionQCall;
   tSettings.mServerRxMsgQCall = mRxMsgQCall;

   // Create the child thread with the settings.
   mTcpMsgServerThread = new Ris::Net::TcpMsgServerThread(tSettings);

   // Launch the child thread.
   mTcpMsgServerThread->launchThread();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immediately
// before the thread is terminated. It shuts down the child thread.

void  ServerThread::threadExitFunction()
{
   Prn::print(Prn::ThreadInit1, "ServerThread::threadExitFunction");

   // Shutdown the child thread.
   mTcpMsgServerThread->shutdownThread(); 
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// QCall registered to the mTcpMsgThread child thread. It is invoked when
// a session is established or disestablished (when a client connects or
// disconnects). It maintains session state variables.

void ServerThread::executeSession (int aSessionIndex,bool aConnected)
{                                                            
   if (aConnected) Prn::print(Prn::ThreadRun1, "ServerThread CONNECTED     %d",aSessionIndex);
   else            Prn::print(Prn::ThreadRun1, "ServerThread DISCONNECTED  %d",aSessionIndex);

   if(!aConnected)
   {
      // The connection was lost, so remove the session from the state list
      mSessionStateList.remove(aSessionIndex);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// QCall registered to the mTcpMsgThread child thread. It is invoked when
// a message is received. It process the received messages. It calls one of
// the specific receive message handlers.

void ServerThread::executeRxMsg(int aSessionIndex,Ris::ByteContent* aMsg)
{
   if(!aMsg) return;

   BaseMsg* tMsg = (BaseMsg*)aMsg;

   // Message jump table based on message type.
   // Calls corresponding specfic message handler method.
   switch (tMsg->mMessageType)
   {
      case MsgIdT::cTestMsg :
         processRxMsg(aSessionIndex,(TestMsg*)tMsg);
         break;
      case MsgIdT::cFirstMessageMsg :
         processRxMsg(aSessionIndex,(FirstMessageMsg*)tMsg);
         break;
      case MsgIdT::cEchoRequestMsg :
         processRxMsg(aSessionIndex,(EchoRequestMsg*)tMsg);
         break;
      case MsgIdT::cEchoResponseMsg:
         processRxMsg(aSessionIndex,(EchoResponseMsg*)tMsg);
         break;
      case MsgIdT::cDataMsg:
         processRxMsg(aSessionIndex,(DataMsg*)tMsg);
         break;
      default :
         Prn::print(Prn::ThreadRun1, "ServerThread::processRxMsg %d",tMsg->mMessageType);
         delete aMsg;
         break;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - TestMsg.

void ServerThread::processRxMsg(int aSessionIndex,TestMsg* aMsg)
{
   Prn::print(Prn::ThreadRun1, "processRxMsg_TestMsg");
   delete aMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - FirstMessageMsg.
//
// Specfic message handler for a FirstMessage. It adds the session to the 
// session state list. This message is sent by the client when a connection
// is established.

void ServerThread::processRxMsg(int aSessionIndex,FirstMessageMsg* aMsg)
{
   Prn::print(Prn::ThreadRun1, "processRxMsg_FirstMessageMsg %d %d",aSessionIndex,aMsg->mHeader.mSourceId);

   // Add session to state list.
   mSessionStateList.add(aSessionIndex,aMsg->mHeader.mSourceId);

   // Delete the message
   delete aMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - EchoRequestMsg.

void ServerThread::processRxMsg(int aSessionIndex,EchoRequestMsg* aMsg)
{
   Prn::print(Prn::ThreadRun1, "processRxMsg_EchoRequestMsg %d %d", aMsg->mCode1, aMsg->mNumWords);

   EchoResponseMsg* tTxMsg = new EchoResponseMsg;
   tTxMsg->mCode1 = aMsg->mCode1;
   tTxMsg->mNumWords = aMsg->mNumWords;
   sendMsg(aSessionIndex, tTxMsg);

   delete aMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - EchoResponseMsg.

void ServerThread::processRxMsg(int aSessionIndex, EchoResponseMsg* aMsg)
{
   Prn::print(Prn::ThreadRun1, "processRxMsg_EchoResponseMsg %d %d", aMsg->mCode1, aMsg->mNumWords);
   delete aMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - DataMsg.

void ServerThread::processRxMsg(int aSessionIndex, DataMsg* aMsg)
{
   Prn::print(Prn::ThreadRun1, "processRxMsg_DataMsg");
   MsgHelper::show(aMsg);
   delete aMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a message via the child thread.

void ServerThread::sendMsg(int aSessionIndex,BaseMsg* aMsg)
{
   // Send a message on socket at the session index.
   mTcpMsgServerThread->sendMsg(aSessionIndex,aMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a message via the child thread.

void ServerThread::sendTestMsg(int aAppNumber)
{
   // Get the session index associated with the application number.
   int tSessionIndex = mSessionStateList.getIndex(aAppNumber);
   if (tSessionIndex == Ris::Net::SessionStateList::cInvalidValue) return;

   // Send a message on socket at the session index.
   TestMsg* tMsg = new TestMsg;
   tMsg->mCode1=201;

   mTcpMsgServerThread->sendMsg(tSessionIndex,tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Execute periodically. This is called by the base class timer. It
// sends an echo request message.

void ServerThread::executeOnTimer(int aTimerCount)
{
   if (!mTPFlag) return;
   Prn::print(Prn::ThreadRun3, "ServerThread::executeOnTimer %d", aTimerCount);

   // Send a status request message.
   EchoRequestMsg* tMsg = new EchoRequestMsg;
   tMsg->mCode1 = aTimerCount;
   tMsg->mNumWords = gTcpSettings.mNumWords;
   sendMsg(0,tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
