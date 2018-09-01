#pragma once

/*==============================================================================
Tcp message socket class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risPortableTypes.h"
#include "risByteContent.h"
#include "risByteMsgMonkey.h"
#include "risContainers.h"
#include "risSockets.h"
#include "risThreadsThreads.h"

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
// Tcp stream socket, blocking.
//
// For a client, it connects (connect) to a Tcp server hub socket and
// exchanges messages (send and recv) with a server stream socket.
//
// For a server, it exchanges messages (send and recv) with a client stream
// socket.
//
// It inherits from BaseTcpStreamSocket for stream socket functionality and
// provides methods that can be used to transport messages.
//
// Messages are based on the ByteContent message encapsulation scheme.

class TcpMsgSocket : public Sockets::BaseTcpStreamSocket
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // This a  message monkey that is used to get details about  a message from
   // a message header that is contained in a byte buffer. For receive, the 
   // message monkey allows the doRecvMsg method to receive and extract a
   // message from a byte buffer without the having the message code visible
   // to it. For transmit, message monkey allows the doSendMsg method to set
   // header data before the message is sent.
   BaseMsgMonkey* mMonkey;

   // Transmit mutex is used by doSendMsg for mutual exclusion.
   Threads::MutexSemaphore  mTxMutex;

   // General purpose valid flag.
   bool mValidFlag;

   // Metrics.
   int mTxMsgCount;
   int mRxMsgCount;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Infastrcture.

   // Constructor.
   TcpMsgSocket(); 
  ~TcpMsgSocket(); 

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Do socket and bind calls.
   void configure(
      BaseMsgMonkeyCreator*  aMonkeyCreator,
      Sockets::SocketAddress aSocketAddress);

   // Do socket and bind calls.
   void configure(
      BaseMsgMonkeyCreator*  aMonkeyCreator);

   // Reconfigure.
   void reconfigure(); 

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Receive a message from the socket via blocking recv calls.
   // It returns true if successful.
   bool doReceiveMsg (ByteContent*& aRxMsg);

   // Send a message over the socket via a blocking send call.
   // It returns true if successful.
   // It is protected by the transmit mutex.
   bool doSendMsg (ByteContent*  aTxMsg);

};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

