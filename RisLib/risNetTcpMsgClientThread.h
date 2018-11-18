#pragma once

/*==============================================================================
Tcp message client thread class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risThreadsQCallThread.h"
#include "risNetSettings.h"
#include "risNetTcpMsgSocket.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Tcp message client thread.
//
// This is a thread that provides the execution context for a tcp client that
// connects to a tcp server and that communicates byte content messages with
// it.
//
// It contains a stream socket that does socket connect calls to establish a
// connection with a server and then does send/recv calls to exchange data
// with it.
   
// The data that is communicated via the socket is encapsulated according to
// the byte content messaging scheme. It sends and receives byte content
// messages.
//
// The thread is structured around a while loop that does either a connect
// call or a recv call to do one of three things: to establish a connection
// to a server, to detect if the connection has been lost, and to receive data
// from the server.
//
// The thread also provides a transmit method that can be used to send
// a message out the socket. The method uses a mutex semaphore for mutual
// exclusion and a blocking send call executes in the context of the calling
// thread.
//
// The thread provides serialized access to the socket and associated state
// variables and it provides the context for the blocking of the recv call.
//
// An instance of this thread is created as a child thread of a parent thread
// that performs message processing. The parent creates the child and
// registers two qcall callbacks to it: one to receive session connection
// change notifications and one to receive messages. When the child thread
// detects a session connect or disconnect it invokes the session qcall to
// notify the parent. When the child thread receives a message it invokes a
// message qcall to pass it to the parent for processing.

class TcpMsgClientThread : public Ris::Threads::BaseThreadWithTermFlag
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
   TcpMsgSocket mSocket;

   // This is a qcall that is invoked when a session is established or
   // disestablished. It is registered by the parent thread at initialzation.
   typedef Ris::Threads::QCall1<bool> SessionQCall;
   SessionQCall mSessionQCall;

   // This is a qcall that is invoked when a message is received.
   // It is registered by the parent thread at initialzation.
   typedef Ris::Threads::QCall1<Ris::ByteContent*> RxMsgQCall;
   RxMsgQCall mRxMsgQCall;

   // If this flag is true then a connection has been established with the 
   // server and sendMsg can be called.
   bool mConnectionFlag;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.
   
   // Constructor.
   TcpMsgClientThread(Settings& aSettings);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods, thread base class overloads:

   // Setup the socket.
   void threadInitFunction()override;

   // Execute a while loop that does connect and recv calls. The loop exits
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
   
   // Notify the parent thread that a session has changed. This is called by
   // the threadRunFunction when a new session is established or an existing
   // session is disestablished. It invokes the mSessionQCall that is
   // registered at initialization.
   virtual void processSessionChange (bool aEstablished);

   // Pass a received message to the parent thread. This is called by the
   // threadRunFunction when a message is received. It invokes the
   // mRxMsgQCall that is registered at initialization.
   virtual void processRxMsg (Ris::ByteContent* aMsg);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.
   
   // Send a transmit message through the socket to the server. It executes a
   // blocking send call in the context of the calling thread. It is protected
   // by a mutex semaphore.
   void sendMsg(ByteContent* aTxMsg);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

