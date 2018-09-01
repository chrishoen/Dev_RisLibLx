//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "prnPrint.h"

#include "risNetUdpMsgSocket.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

UdpRxMsgSocket::UdpRxMsgSocket()
{
   mRxLength = 0;
   mRxCount = 0;
   mValidFlag = false;
   mMonkey = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

UdpRxMsgSocket::~UdpRxMsgSocket()
{
   if (mMonkey != 0)
   {
      delete mMonkey;
      mMonkey = 0;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Configure the socket.

void UdpRxMsgSocket::configure(
   BaseMsgMonkeyCreator* aMonkeyCreator,
   char*                 aLocalIpAddr,
   int                   aLocalIpPort)
{
   mRxCount = 0;
   mRxLength = 0;

   mLocal.set(aLocalIpAddr, aLocalIpPort);
   mMonkey = aMonkeyCreator->createMonkey();

   doSocket();
   doBind();

   if (mStatus == 0)
   {
      Prn::print(Prn::SocketInit2, "UdpRxMsgSocket     $ %16s : %d",
         mLocal.mIpAddr.mString,
         mLocal.mPort);
   }
   else
   {
      Prn::print(Prn::SocketInit2, "UdpRxMsgSocket     $ %16s : %d $ %d %d",
         mLocal.mIpAddr.mString,
         mLocal.mPort,
         mStatus,
         mError);
   }

   mValidFlag = mStatus == 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive a datagram from the socket into a byte buffer and then extract a 
// message from it.

bool UdpRxMsgSocket::doReceiveMsg(ByteContent*& aMsg)
{
   //---------------------------------------------------------------------------
   // Initialize
   aMsg = 0;

   // Guard.
   if (!mValidFlag) return false;

   // Create a byte buffer.
   ByteBuffer tBuffer(mMonkey->getMaxBufferSize());

   // Configure the byte buffer.
   mMonkey->configureByteBuffer(&tBuffer);
   tBuffer.setCopyFrom();

   //---------------------------------------------------------------------------
   // Read the message into the receive buffer.

   doRecvFrom(mFromAddress, tBuffer.getBaseAddress(), mRxLength, mMonkey->getMaxBufferSize());

   // Guard.
   // If bad status then return false.
   // Returning true  means socket was not closed.
   // Returning false means socket was closed.

   if (mRxLength <= 0)
   {
      Prn::print(Prn::SocketRun1, "UdpRxMsgSocket ERROR  %d %d", mStatus, mError);
      if (mStatus < 0)
         switch (mError)
         {
         case 0: return false; break;
         default: return false; break;
         }
      else
         return false;
   }

   Prn::print(Prn::SocketRun2, "UdpRxMsgSocket rx message %d", mRxLength);

   // Set the buffer length.
   tBuffer.setLength(mRxLength);

   //---------------------------------------------------------------------------
   // Copy from the receive buffer into the message monkey object and validate
   // the header.

   mMonkey->extractMessageHeaderParms(&tBuffer);

   Prn::print(Prn::SocketRun3, "UdpRxMsgSocket rx header %d %d",
      mMonkey->mHeaderValidFlag,
      mMonkey->mHeaderLength);

   // If the header is not valid then error.
   if (!mMonkey->mHeaderValidFlag)
   {
      Prn::print(Prn::SocketRun1, "UdpRxMsgSocket ERROR INVALID HEADER", mStatus, mError);
      return false;
   }

   //---------------------------------------------------------------------------
   // At this point the buffer contains the complete message. Extract the
   // message from the byte buffer into a new message object and return it.

   tBuffer.rewind();
   aMsg = mMonkey->getMsgFromBuffer(&tBuffer);

   // Test for errors.
   if (aMsg == 0)
   {
      Prn::print(Prn::SocketRun1, "UdpRxMsgSocket ERROR INVALID MESSAGE", mStatus, mError);
      mStatus = tBuffer.getError();
      return false;
   }

   // Returning true  means socket was not closed.
   // Returning false means socket was closed.
   mRxCount++;
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

UdpTxMsgSocket::UdpTxMsgSocket()
{
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
   if (mMonkey) delete mMonkey;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Configure the socket.

void UdpTxMsgSocket::configure(
   BaseMsgMonkeyCreator* aMonkeyCreator,
   char*                 aRemoteIpAddr,
   int                   aRemoteIpPort)
{
   mTxCount = 0;

   mRemote.set(aRemoteIpAddr, aRemoteIpPort);
   mMonkey = aMonkeyCreator->createMonkey();

   doSocket();

   if (mStatus == 0)
   {
      Prn::print(Prn::SocketInit2, "UdpTxMsgSocket     $ %16s : %d",
         mRemote.mIpAddr.mString,
         mRemote.mPort);
   }
   else
   {
      Prn::print(Prn::SocketInit2, "UdpTxMsgSocket     $ %16s : %d $ %d %d",
         mRemote.mIpAddr.mString,
         mRemote.mPort,
         mStatus,
         mError);
   }

   mValidFlag = mStatus == 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Copy a message into a byte buffer and then send the byte buffer out the 
// socket.

bool UdpTxMsgSocket::doSendMsg(ByteContent* aMsg)
{
   // Guard.
   if (!mValidFlag) return false;

   // Create a byte buffer.
   ByteBuffer tBuffer(mMonkey->getMaxBufferSize());

   // Configure the byte buffer.
   mMonkey->configureByteBuffer(&tBuffer);
   tBuffer.setCopyTo();

   // Copy the message to the buffer.
   mMonkey->putMsgToBuffer(&tBuffer, aMsg);

   // Delete the message.
   delete aMsg;

   // Mutex.
   mTxMutex.lock();

   // Transmit the buffer.
   mTxLength = tBuffer.getLength();
   doSendTo(mRemote, tBuffer.getBaseAddress(), mTxLength);

   Prn::print(Prn::SocketRun2, "UdpTxMsgSocket tx message %d", mTxLength);

   // Mutex.
   mTxMutex.unlock();

   // Done.
   mTxCount++;
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

