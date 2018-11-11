//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risNetUdpMsgSocket.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

UdpRxMsgSocket::UdpRxMsgSocket()
{
   mRxMemory = 0;
   mMemorySize = 0;
   mRxLength = 0;
   mRxCount = 0;
   mValidFlag = false;
   mMonkey = 0;
}

UdpRxMsgSocket::~UdpRxMsgSocket()
{
   if (mRxMemory) free(mRxMemory);
   if (mMonkey) delete mMonkey;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize variables.

void UdpRxMsgSocket::initialize(Settings& aSettings)
{
   // Store the settings pointer.
   mSettings = aSettings;

   // Create a message monkey.
   mMonkey = mSettings.mMonkeyCreator->createMonkey();

   // Allocate memory for byte buffers.
   mMemorySize = mMonkey->getMaxBufferSize();
   mRxMemory = (char*)malloc(mMemorySize);

   // Metrics.
   mRxCount = 0;
   mRxLength = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Configure the socket.

void UdpRxMsgSocket::configure()
{
   // Configure the socket.
   BaseClass::mLocal.set(mSettings.mLocalIpAddr, mSettings.mLocalIpPort);
   BaseClass::doSocket();
   BaseClass::doBind();

   // Set valid flag from base class results.
   mValidFlag = BaseClass::mStatus == 0;

   // Show.
   if (mValidFlag)
   {
      TS::print(1, "UdpRxMsgSocket     PASS %16s : %5d",
         BaseClass::mLocal.mIpAddr.mString,
         BaseClass::mLocal.mPort);
   }
   else
   {
      TS::print(1, "UdpRxMsgSocket     FAIL %16s : %5d $ %d %d",
         BaseClass::mLocal.mIpAddr.mString,
         BaseClass::mLocal.mPort,
         BaseClass::mStatus,
         BaseClass::mError);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive a message from the socket with a blocking recv call into a
// byte buffer and extract a message from the byte buffer. Return the
// message and true if successful. As part of the termination process,
// returning false means that the socket was closed or that there was
// an error.

bool UdpRxMsgSocket::doReceiveMsg(ByteContent*& aMsg)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize.

   // Do this first.
   aMsg = 0;

   // Guard.
   if (!mValidFlag)
   {
      TS::print(0, "ERROR UdpRxMsgSocket INVALID SOCKET");
      return false;
   }

   // Create a byte buffer from preallocated memory.
   ByteBuffer tByteBuffer(mRxMemory, mMemorySize);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read the message into the receive buffer.

   tByteBuffer.setCopyFrom();
   BaseClass::doRecvFrom(mFromAddress, tByteBuffer.getBaseAddress(), mRxLength, mMemorySize);

   // Guard.
   // If bad status then return false.
   // Returning true  means socket was not closed.
   // Returning false means socket was closed.
   if (mRxLength <= 0)
   {
      if (BaseClass::mError == 0)
      {
         TS::print(1, "UdpRxMsgSocket CLOSED");
      }
      else
      {
         TS::print(0, "ERROR UdpRxMsgSocket %d %d", BaseClass::mStatus, BaseClass::mError);
      }
      return false;
   }

   TS::print(3, "UdpRxMsgSocket rx message %d", mRxLength);

   // Set the buffer length.
   tByteBuffer.setLength(mRxLength);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Copy from the receive buffer into the message monkey and validate
   // the header.

   // Extract the header.
   mMonkey->extractMessageHeaderParms(&tByteBuffer);

   TS::print(3, "UdpRxMsgSocket rx header %d %d",
      mMonkey->mHeaderValidFlag,
      mMonkey->mHeaderLength);

   // If the header is not valid then error.
   if (!mMonkey->mHeaderValidFlag)
   {
      TS::print(0, "ERROR UdpRxMsgSocket INVALID HEADER", mStatus, mError);
      return false;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // At this point the buffer contains the complete message. Extract the
   // message from the byte buffer into a new message object and return it.

   // Extract the message.
   tByteBuffer.rewind();
   aMsg = mMonkey->getMsgFromBuffer(&tByteBuffer);

   // Test for errors.
   if (aMsg == 0)
   {
      TS::print(0, "ERROR UdpRxMsgSocket INVALID MESSAGE", mStatus, mError);
      mStatus = tByteBuffer.getError();
      return false;
   }

   // Done.
   mRxCount++;
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

UdpTxMsgSocket::UdpTxMsgSocket()
{
   mTxMemory = 0;
   mMemorySize = 0;
   mTxCount = 0;
   mTxLength = 0;
   mValidFlag = false;
   mMonkey = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

UdpTxMsgSocket::~UdpTxMsgSocket()
{
   if (mTxMemory) free(mTxMemory);
   if (mMonkey) delete mMonkey;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize variables.

void UdpTxMsgSocket::initialize(Settings& aSettings)
{
   // Store the settings.
   mSettings = aSettings;

   // Create a message monkey.
   mMonkey = mSettings.mMonkeyCreator->createMonkey();

   // Allocate memory for byte buffers.
   mMemorySize = mMonkey->getMaxBufferSize();
   mTxMemory = (char*)malloc(mMemorySize);

   // Metrics.
   mTxCount = 0;
   mTxLength = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Configure the socket.

void UdpTxMsgSocket::configure()
{
   // Configure the socket.
   BaseClass::mRemote.set(mSettings.mRemoteIpAddr, mSettings.mRemoteIpPort);
   BaseClass::doSocket();

   // Set valid flag from base class results.
   mValidFlag = BaseClass::mStatus == 0;

   // Show.
   if (mValidFlag)
   {
      TS::print(1, "UdpTxMsgSocket     PASS %16s : %5d",
         BaseClass::mRemote.mIpAddr.mString,
         BaseClass::mRemote.mPort);
   }
   else
   {
      TS::print(1, "UdpTxMsgSocket     FAIL %16s : %5d $ %d %d",
         BaseClass::mRemote.mIpAddr.mString,
         BaseClass::mRemote.mPort,
         BaseClass::mStatus,
         BaseClass::mError);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Copy a message into a byte buffer and then send the byte buffer to the
// socket with a blocking send call. Return true if successful.
// It is protected by the transmit mutex.

bool UdpTxMsgSocket::doSendMsg(ByteContent* aMsg)
{
   // Guard.
   if (!mValidFlag)
   {
      TS::print(0, "ERROR UdpTxMsgSocket INVALID SOCKET");
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
   bool tRet = doSendTo(mRemote, tByteBuffer.getBaseAddress(), mTxLength);
   mTxCount++;

   // Mutex.
   mTxMutex.unlock();

   if (tRet)
   {
      TS::print(3, "UdpTxMsgSocket tx message %d", mTxLength);
   }
   else
   {
      TS::print(0, "ERROR UdpTxMsgSocket INVALID SEND");
   }

   // Done.
   return tRet;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

