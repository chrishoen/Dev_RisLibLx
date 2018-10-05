#pragma once

/*==============================================================================
Udp message thread.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risThreadsQCallThread.h"
#include "risNetUdpMsgSocket.h"
#include "risNetSettings.h"

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
// that communicates byte content messages with another udp peer.
//
// It contains a receive  udp socket that is bound to a local address.
// It contains a transmit udp socket that is bound to a remote address.
//
// The data that is communicated via the sockets is encapsulated according to
// the byte content messaging scheme. It sends and receives byte content
// messages.
//
// The thread is structured around a while loop that does a recvfrom
// call to receive a message on the socket.
//
// The thread provides serialized access to the socket and associated 
// state variables and it provides the context for the blocking of the 
// recv call.
//
// An instance of this thread is created as a child thread of a parent thread
// that performs message processing. The parent creates the child and
// registers a receive message qcall callback to it. When the child thread
// receives a message it invokes the message qcall to pass it to the parent
// for processing.

class UdpMsgThread : public Ris::Threads::BaseThreadWithTermFlag
{
public:
   typedef Ris::Threads::BaseThreadWithTermFlag BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Settings.
   Settings mSettings;

   // Socket instance.
   UdpRxMsgSocket mRxSocket;
   UdpTxMsgSocket mTxSocket;

   // This is a qcall callback that is invoked when a message is received.
   // It is registered by the parent thread at initialzation.
   typedef Ris::Threads::QCall1<Ris::ByteContent*> RxMsgQCall;
   RxMsgQCall mRxMsgQCall;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   UdpMsgThread(Settings& aSettings);

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

   // Pass a received message to the parent thread. This is called by the
   // threadRunFunction when a message is received. It invokes the
   // mRxMsgQCall that is registered at initialization.
   virtual void processRxMsg(Ris::ByteContent* aMsg);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Send a transmit message through the socket to the peer. It executes a
   // blocking send call in the context of the calling thread. It is protected
   // by a mutex semaphore.
   void sendMsg(Ris::ByteContent* aMsg);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

