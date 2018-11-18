#pragma once

/*==============================================================================
Tcp message server thread class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risContainers.h"
#include "risThreadsQCallThread.h"
#include "risNetSettings.h"
#include "risNetTcpServerHubSocket.h"
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
// Tcp message server thread.

// This is a thread that provides the execution context for a tcp server that
// can service multiple clients and communicate byte content messages with
// them.
//
// It contains a hub socket that does socket listen and accept calls and a set
// of node sockets that do send/recv calls to/from corresponding client stream
// sockets.
//
// The data that is communicated via the sockets is encapsulated according to
// the byte content messaging scheme. It sends and receives byte content
// messages.
//
// The thread is structured around a socket select call that blocks until one
// of three things happens: 1) a hub socket accept is available, 2) a node
// socket read is available, or 3) a one second timeout has 
// occurred.
// 
// If a hub socket accept is available then a nonblocking accept call is 
// executed and a new session is established for one of the node sockets. If
// the number of sessions has reached a maximum then the hub socket is closed
// until one of the sessions is closed.
// 
// If a node socket read is available then a nonblocking recv call is 
// executed on the indicated node socket and a message is extracted from
// the read buffer and processed. If there was an error on the recv call then
// this indicates that the connection was closed by the client, so the session
// is disestablished and the node socket is closed.
//
// If a timeout occurs then the thread terminate is polled and the thread
// terminates if requested to do so.
//
// The thread also provides a transmit method that can be used to send
// a message out one of the node sockets. The method uses mutex semaphores
// for mutual exclusion and the send call executes in the context of the
// calling thread.
//
// The thread provides serialized access to the sockets and associated 
// state variables and it provides the context for the blocking of the 
// select call.
//
// An instance of this thread is created as a child thread of a parent thread
// that performs message processing. The parent creates the child and
// registers two qcall callbacks to it: one to receive session connection
// change notifications and one to receive messages. When the child thread
// detects a session connect or disconnect it invokes the session qcall to
// notify the parent. When the child thread receives a message it invokes a
// message qcall to pass it to the parent for processing.

class TcpMsgServerThread : public Threads::BaseThreadWithTermFlag
{
public:
   typedef Ris::Threads::BaseThreadWithTermFlag BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants.

   // Maximum number of sessions.
   static const int cMaxSessions = 20;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Settings.
   Settings mSettings;

   // Hub socket instance.
   TcpServerHubSocket mHubSocket;

   // Node socket instances.
   // Access this array with a session index and test the node socket
   // valid flag.
   TcpMsgSocket mNodeSocket[cMaxSessions];

   // This is a qcall that is called when a session is established or
   // disestablished. It is registered by the parent thread at initialzation.
   typedef Ris::Threads::QCall2<int, bool> SessionQCall;
   SessionQCall mSessionQCall;

   // This is a qcall that is called when a message is received.
   // It is registered by the parent thread at initialzation.
   typedef Ris::Threads::QCall2<int,Ris::ByteContent*> RxMsgQCall;
   RxMsgQCall mRxMsgQCall;

   // Maximum configured number of sessions.
   // 1 < mMaxSessions <= cMaxSessions
   int mMaxSessions; 

   // Socket address that the hub socket binds to.
   Sockets::SocketAddress mSocketAddress;

   // mSessionAllocator.get() allocates a new session index.
   // mSessionAllocator.put() deallocates a   session index.
   // This is used at the accept() call to allocate a new
   // session index for a node socket. 
   Containers::Allocator<cMaxSessions> mSessionAllocator;

   // Number of active sessions.
   int  mNumSessions;

   // If this flag is true then the hub socket is open and listening
   // for new client connections.
   // If this flag is false then the number of active sessions is at 
   // the maximum and the hub socket has been closed and is not
   // listening and new client connections will be refused.
   bool mListenFlag;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   TcpMsgServerThread(Settings& aSettings);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods, thread base class overloads:

   // Setup the hub socket.
   void threadInitFunction()override;

   // Execute a select call to process accepts on the hub socket
   // and recvs on the node sockets.
   void threadRunFunction()override;

   // Close the hub socekt and all open node sockets..
   void threadExitFunction()override;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Notify the parent thread that a session has changed. This is called by
   // the threadRunFunction when a new session is established or an existing
   // session is disestablished. It invokes the mSessionQCall that is
   // registered at initialization.
   void processSessionChange(int aSessionIndex,bool aEstablished);

   // Pass a received message to the parent thread. This is called by the
   // threadRunFunction when a message is received. It invokes the
   // mRxMsgQCall that is registered at initialization.
   void processRxMsg (int aSessionIndex,Ris::ByteContent* aMsg);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Send a transmit message through the node socket at the session index
   // to the client. It executes a blocking send call in the context of
   // the calling thread. It is protected by a mutex semaphore.
   void sendMsg(int aSessionIndex, ByteContent* aMsg);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

