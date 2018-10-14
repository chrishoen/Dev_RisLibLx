/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "prnPrint.h"

#include "risNetTcpMsgSocket.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.
   
TcpMsgSocket::TcpMsgSocket()
{
   mTxMemory = 0;
   mRxMemory = 0;
   mMemorySize = 0;
   mTxLength = 0;
   mRxLength = 0;
   mTxCount = 0;
   mRxCount = 0;
   mValidFlag = false;
   mMonkey = 0;
}

TcpMsgSocket::~TcpMsgSocket()
{
   if (mTxMemory) free(mTxMemory);
   if (mRxMemory) free(mRxMemory);
   if (mMonkey) delete mMonkey;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize variables.

void TcpMsgSocket::initialize(Settings& aSettings)
{
   // Store the settings pointer.
   mSettings = aSettings;

   // Create a message monkey.
   mMonkey = mSettings.mMonkeyCreator->createMonkey();

   // Allocate memory for byte buffers.
   mMemorySize = mMonkey->getMaxBufferSize();
   mTxMemory = (char*)malloc(mMemorySize);
   mRxMemory = (char*)malloc(mMemorySize);

   // Not valid until configured.
   mValidFlag = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Configure the socket.

void TcpMsgSocket::configure()
{
   // Configure the socket.
   BaseClass::reset();
   BaseClass::mRemote.set(mSettings.mRemoteIpAddr, mSettings.mRemoteIpPort);
   BaseClass::doSocket();
   BaseClass::setOptionKeepAlive();
   BaseClass::setOptionNoDelay();

   // Set valid flag from base class results.
   mValidFlag = BaseClass::mStatus == 0;

   // Show.
   if (mValidFlag)
   {
      Prn::print(Prn::SocketInitS1, "TcpMsgSocket            %16s : %5d",
         BaseClass::mRemote.mIpAddr.mString,
         BaseClass::mRemote.mPort);
   }
   else
   {
      Prn::print(Prn::SocketErrorS1, "TcpMsgSocket       FAIL %16s : %5d $ %d %d",
         BaseClass::mRemote.mIpAddr.mString,
         BaseClass::mRemote.mPort,
         BaseClass::mStatus,
         BaseClass::mError);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Reconfigure the socket.

void TcpMsgSocket::reconfigure()
{
   // Configure the socket.
   BaseClass::doSocket();
   BaseClass::setOptionKeepAlive();
   BaseClass::setOptionNoDelay();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Copy a message into a byte buffer and then send the byte buffer to the
// socket with a blocking send call. Return true if successful.
// It is protected by the transmit mutex.

bool TcpMsgSocket::doSendMsg(ByteContent* aMsg)
{
   // Guard.
   if (!mValidFlag)
   {
      Prn::print(Prn::SocketErrorS1, "ERROR TcpMsgSocket INVALID SOCKET");
      delete aMsg;
      return false;
   }

   // Mutex.
   mTxMutex.lock();

   // Create a byte buffer from preallocated memory.
   ByteBuffer tByteBuffer(mTxMemory, mMemorySize);

   // Copy the message to the buffer.
   mMonkey->putMsgToBuffer(&tByteBuffer, aMsg);

   // Delete the message.
   delete aMsg;

   // Transmit the buffer.
   mTxLength = tByteBuffer.getLength();
   bool tRet = doSend(tByteBuffer.getBaseAddress(), mTxLength);
   mTxCount++;

   // Mutex.
   mTxMutex.unlock();

   if (tRet)
   {
      Prn::print(Prn::SocketTxRunS1, "TcpMsgSocket tx message %d", mTxLength);
   }
   else
   {
      Prn::print(Prn::SocketErrorS1, "ERROR TcpMsgSocket INVALID SEND");
   }

   // Done.
   return tRet;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive a message from the socket with a blocking recv call into a
// byte buffer and extract a message from the byte buffer. Return the
// message and true if successful. As part of the termination process,
// returning false means that the socket was closed or that there was
// an error.

bool TcpMsgSocket::doReceiveMsg (ByteContent*& aMsg)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize a receive byte buffer.

   // Do this first.
   aMsg=0;
   bool tRet=false;
   int tStatus=0;

   // Guard.
   if (!mValidFlag)
   {
      Prn::print(Prn::SocketErrorS1, "ERROR TcpMsgSocket INVALID SOCKET");
      return false;
   }

   // Create a byte buffer from preallocated memory.
   ByteBuffer tByteBuffer(mRxMemory, mMemorySize);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read the message header into the receive buffer.

   // Header varaibles.
   int   tHeaderLength = mMonkey->getHeaderLength();
   char* tHeaderBuffer = tByteBuffer.getBaseAddress();

   // Read the header from the socket.
   tByteBuffer.setCopyTo();
   tRet = BaseClass::doRecv(tHeaderBuffer,tHeaderLength,tStatus);
   Prn::print(Prn::SocketRxRunS2, "doRecvH %d %d",mStatus,mError);

   // Guard.
   // If bad status then return false.
   // Returning true  means socket was not closed.
   // Returning false means socket was closed.
   if (!tRet || tStatus<=0)
   {
      Prn::print(Prn::SocketRxRunS2, "ERROR TcpMsgSocket INVALID READ");
      return false;
   }

   // Set the buffer length.
   tByteBuffer.setLength(tHeaderLength);

   // Copy from the receive buffer into the message monkey 
   // and validate the header.
   mMonkey->extractMessageHeaderParms(&tByteBuffer);

   // If the header is not valid then error.
   if (!mMonkey->mHeaderValidFlag)
   {
      Prn::print(Prn::SocketErrorS1, "ERROR TcpMsgSocket INVALID HEADER");
      return false;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read the message payload into the receive buffer.

   // Payload variables.
   int   tPayloadLength = mMonkey->mPayloadLength;
   char* tPayloadBuffer = tByteBuffer.getBaseAddress() + tHeaderLength;

   // Read the payload from the socket.
   tByteBuffer.setCopyTo();
   tRet = BaseClass::doRecv(tPayloadBuffer,tPayloadLength,tStatus);
   Prn::print(Prn::SocketRxRunS2, "doRecvP %d %d %d",mStatus,mError,tPayloadLength);

   // If bad status then return false.
   if (!tRet || tStatus<=0)
   {
      Prn::print(Prn::SocketErrorS1, "ERROR doRecv2 INVALID RECV");
      return false;
   }

   // Set the buffer length.
   tByteBuffer.setLength(mMonkey->mMessageLength);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // At this point the buffer contains the complete message. Extract the 
   // message from the byte buffer into a new message object and return it.

   // Extract the message.
   tByteBuffer.rewind();
   aMsg = mMonkey->getMsgFromBuffer(&tByteBuffer);

   // Test for errors.
   if (aMsg==0)
   {
      Prn::print(Prn::SocketErrorS1, "ERROR TcpMsgSocket INVALID MESSAGE");
      mStatus=tByteBuffer.getError();
      return false;
   }

   // Done.
   mRxCount++;
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

