#pragma once

/*==============================================================================
Udp receiver thread.
==========================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risCallPointer.h"
#include "risContainers.h"
#include "risSockets.h"
#include "risThreadsThreads.h"
#include "risThreadsQCallThread.h"
#include "risNetUdpMsgSocket.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Udp message thread.
//
// This is a thread that provides the execution context for a udp peer that 
// communicates with another udp peer.
//
// It contains a receive  udp socket that is bound to a local address.
// It contains a transmit udp socket that is bound to a remote address.
//
// The thread is structured around a while loop that does a recvfrom
// call to receive a message on the socket.
//
// The thread provides serialized access to the socket and associated 
// state variables and it provides the context for the blocking of the 
// recv call.

class UdpMsgThread : public Ris::Threads::BaseThreadWithTermFlag
{
public:
   typedef Ris::Threads::BaseThreadWithTermFlag BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   typedef Ris::Threads::QCall1<Ris::ByteContent*> RxMsgQCall;

   // This is a qcall callback that is called when a message is received.
   RxMsgQCall mRxMsgQCall;

   // Socket address that socket instance connects to.
   char  mLocalIpAddress[40];
   int   mLocalIpPort;
   char  mRemoteIpAddress[40];
   int   mRemoteIpPort;

   // Socket instance.
   UdpRxMsgSocket mRxSocket;
   UdpTxMsgSocket mTxSocket;

   // Message monkey creator, this is used by the two sockets to create an
   // instance of a message monkey.
   BaseMsgMonkeyCreator* mMonkeyCreator;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Infastrcture.

   // Constructor.
   UdpMsgThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Configure the thread.
   // aMonkeyCreator  creates a message monkey to be used on messages
   // aLocalIpAddr    is the ip address of the local  interface bound to
   // aLocalIpPort    is the ip port    of the local  interface bound to
   // aRemoteIpAddr   is the ip address of the remote interface bound to
   // aRemoteIpPort   is the ip port    of the remote interface bound to
   // aRxMsgQCall     is a qcall for receive messages

   void configure(
      Ris::BaseMsgMonkeyCreator* aMonkeyCreator, 
      char*                      aLocalIpAddress,
      int                        aLocalIpPort,
      char*                      aRemoteIpAddress,
      int                        aRemoteIpPort,
      RxMsgQCall*                aRxMsgQCall);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods, thread base class overloads:

   // Setup the socket.
   void threadInitFunction()override;

   // Execute a while loop that does recv calls. The loop exits
   // when the socket is closed and the termination flag is true.
   void threadRunFunction()override;

   // Print.
   void threadExitFunction()override;

   // Set the termination flag, close the socket and wait for the thread to
   // terminate.
   void shutdownThread()override; 

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Process a received message. This is called by the threadRunFunction.
   // It invokes the mRxMsgQCall that is passed in at configure to pass the
   // message to the thread owner.
   void processRxMsg (Ris::ByteContent* aMsg);

   // Send a transmit message through the socket. It executes a blocking send
   // call in the context of the caller.
   void sendMsg (Ris::ByteContent* aMsg);

};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

