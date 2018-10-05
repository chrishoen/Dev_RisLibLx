#pragma once

/*==============================================================================
UDP receive  message socket.
UDP transmit message socket.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risByteContent.h"
#include "risByteMsgMonkey.h"
#include "risThreadsSynch.h"
#include "risNetSettings.h"
#include "risSockets.h"

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
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Udp receive message socket. This class encapsulates a udp socket that
// receives messages that are based on the byte content message encapsulation
// scheme.

class UdpRxMsgSocket : public Sockets::BaseUdpSocket
{
public:
   typedef Sockets::BaseUdpSocket BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Settings.
   Settings mSettings;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Receive memory. Allocated at initialization.
   char* mRxMemory;

   // Size of allocated memory.
   int mMemorySize;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // This a message monkey that is used to manage the details about messages
   // and message headers while hiding the underlying specific message set code.
   // For received  messages, the message monkey allows the receive method to
   // extract message header details from a byte buffer and it allows it to 
   // then extract full messages from the byte buffer. For transmited messages,
   // the message monkey allows the send method to set header data before the
   // message is sent. A specific message monkey is provided by the parent 
   // thread at initialization.
   BaseMsgMonkey* mMonkey;

   // True if the socket is valid.
   bool mValidFlag;

   // The recvfrom address is stored here.
   Sockets::SocketAddress mFromAddress;

   // Metrics.
   int mRxLength;
   int mRxCount;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   UdpRxMsgSocket(); 
   ~UdpRxMsgSocket(); 

   // Initialize variables.
   void initialize(Settings& aSettings);

   // Configure the socket. This does socket and bind calls.
   void configure();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Receive a message from the socket with a blocking recv call into a
   // byte buffer and extract a message from the byte buffer. Return the
   // message and true if successful. As part of the termination process,
   // returning false means that the socket was closed or that there was
   // an error.
   bool doReceiveMsg (ByteContent*& aRxMsg);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Udp transmit message socket. This class encapsulates a udp socket that
// transmits messages that are based on the byte content message encapsulation
// scheme.

class UdpTxMsgSocket : public Sockets::BaseUdpSocket
{
public:
   typedef Sockets::BaseUdpSocket BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Settings.
   Settings mSettings;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Transmit memory. Allocated at initialization.
   char* mTxMemory;

   // Size of allocated memory.
   int mMemorySize;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // This is a message monkey that is used to get details about a message 
   // from a message header that is contained in a byte buffer. It allows the 
   // receive method to receive and extract a message from a byte buffer
   // without the having the message code visible to it.
   BaseMsgMonkey* mMonkey;

   // Transmit mutex is used by doSendMsg for mutual exclusion.
   Threads::MutexSemaphore  mTxMutex;

   // General purpose valid flag.
   bool mValidFlag;

   // Metrics.
   int mTxLength;
   int mTxCount;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   UdpTxMsgSocket(); 
   ~UdpTxMsgSocket(); 

   // Initialize variables.
   void initialize(Settings& aSettings);

   // Configure the socket. This does socket and bind calls.
   void configure();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Copy a message into a byte buffer and then send the byte buffer to the
   // socket with a blocking send call. Return true if successful.
   // It is protected by the transmit mutex.
   bool doSendMsg(ByteContent* aMsg);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace


