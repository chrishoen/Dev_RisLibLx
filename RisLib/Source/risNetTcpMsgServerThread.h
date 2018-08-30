#pragma once

/*==============================================================================

Tcp server thread classes.

This file provides classes that define a single thread that provides the 
execution context for a tcp server that connects to a tcp client.

There is a base class and two classes that provide different interfaces.

1) TcpServerThread   provides the tcp server thread functionality

2) TcpServerThreadWithQCall : public TcpServerThread provides a tcp server
   thread with a QCall (deferred procedure call) interface

3) TcpServerThreadWithCallback : public TcpServerThread provides a tcp
   server thread with a callback interface

Threads that want to perform Tcp server activity maintain instances of 
TcpServerThreadWithQCall or TcpServerThreadWithCallback and pass in QCalls
or callbacks in their configure calls.

==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risCallPointer.h"
#include "risContainers.h"
#include "risSockets.h"
#include "risThreadsThreads.h"
#include "risThreadsQCallThread.h"

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
// Tcp server hub socket, it is used to manage client connections.
// It does listen and accept socket calls into tcp stream sockets in response
// to client connect calls. 

class TcpMsgServerHubSocket : public Sockets::BaseTcpServerHubSocket
{
public:
   // Socket setup

   // This stores the socket address and does socket and bind calls.
   void configure(Sockets::SocketAddress aSocketAddress);

   // This does socket and bind calls.
   void reconfigure();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Server thread.
// This is a single thread that provides the execution context for a tcp
// server that can service multiple clients.
//
// It contains a hub socket that does socket listen and accept calls and 
// a set of node sockets that do send/recv calls to/from corresponding 
// client stream sockets.
//
// The thread is structured around a socket select call that blocks until
// one of three things happens: 1) a hub socket accept is available,
// 2) a node socket read is available, or 3) a one second timeout has 
// occurred.
// 
// If a hub socket accept is available then a nonblocking accept call is
// executed and a new session is established for one of the node sockets. 
// If the number of sessions has reached a maximum then the hub socket is 
// closed until one of the sessions is closed.
// 
// If a node socket read is available then a nonblocking recv call is 
// executed on the indicated node socket and a message is extracted from
// the read buffer. If there was an error on the recv call then this
// indicates that the connection was closed by the client, so the session 
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

class TcpMsgServerThread : public Threads::BaseThreadWithTermFlag
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants.

   // Maximum possible number of sessions
   enum {MaxSessions=20};

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   typedef Ris::Threads::QCall2<int,bool>              SessionQCall;
   typedef Ris::Threads::QCall2<int,Ris::ByteContent*> RxMsgQCall;

   // This is a qcall that is called when a session is established or
   // disestablished.
   SessionQCall mSessionQCall;

   // This is a qcall that is called when a message is received.
   RxMsgQCall   mRxMsgQCall;

   // Hub socket instance.
   TcpMsgServerHubSocket mHubSocket;

   // Maximum configured number of sessions.
   // 0 < mMaxSessions <= MaxSessions
   int mMaxSessions; 

   // Node socket instances.
   // Access this array with a session index and test the node socket
   // valid flag.
   TcpMsgSocket mNodeSocket[MaxSessions];

   // Socket address that the hub socket binds to.
   Sockets::SocketAddress mSocketAddress;

   // Message monkey creator for node sockets.
   BaseMsgMonkeyCreator* mMonkeyCreator;

   // mSessionAllocator.get() allocates a new session index.
   // mSessionAllocator.put() deallocates a   session index.
   // This is used at the accept() call to allocate a new
   // session index for a node socket. 
   Containers::Allocator<MaxSessions> mSessionAllocator;

   // Number of active sessions.
   int  mNumSessions;

   // If this flag is true then the hub socket is open and listening
   // for new client connections.
   // If this flag is false then the number of active sessions is at 
   // the maximum and the hub socket has been closed and is not
   // listening and new client connections will be refused.
   bool mListenFlag;

   // Some socket flags.
   int mFlags;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Intrastructure.

   // Constructor.
   TcpMsgServerThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Configure the thread. 
   // aMonkey             is the message monkey to be used on receive messages.
   // aServerIpAddr       is the server ip address.
   // aMaxSessions        is the max number of sessions allowed. 
   // aServerIpPort       is the server ip port.
   // aRxMsgQCall         is a qcall for receive messages.
   // aSessionQCallChange is a qcall for session changes.
   // aFlags              is some socket flags.

   void configure(
      BaseMsgMonkeyCreator* aMonkeyCreator,
      char*                 aServerIpAddr,
      int                   aServerIpPort,
      int                   aMaxSessions, 
      SessionQCall*         aSessionQCall,
      RxMsgQCall*           aRxMsgQCall,
      int                   aFlags=0);

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

   // Notify the thread owner of this thread that a session has changed. This
   // is  called by the threadRunFunction  when a new session is established 
   // or an existing session is disestablished. It invokes the mSessionQCall
   // that is passed in at configure.
   void processSessionChange(int aSessionIndex,bool aEstablished);

   // Pass a received message to the thread owner. It invokes the mRxMsgQCall
   // that is passed in at configure. This is called by the threadRunFunction
   // to process a received message.
   void processRxMsg (int aSessionIndex,Ris::ByteContent* aMsg);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Send a transmit message through the node socket at the session index to the client. It executes
   // a blocking send() call in the context of the caller. It is protected by a
   // mutex semaphore.
   void sendMsg(int aSessionIndex,ByteContent* aMsg);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

