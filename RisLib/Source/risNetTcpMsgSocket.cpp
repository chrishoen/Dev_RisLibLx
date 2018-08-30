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
TcpMsgSocket::TcpMsgSocket()
{
   mTxMsgCount=0;
   mRxMsgCount=0;
   mValidFlag=false;
   mMonkey=0;
}

//******************************************************************************
TcpMsgSocket::~TcpMsgSocket()
{
   if (mMonkey != 0)
   {
      delete mMonkey;
      mMonkey = 0;
   }
}

//******************************************************************************
// Configure the socket

void TcpMsgSocket::configure(
   BaseMsgMonkeyCreator*  aMonkeyCreator,
   Sockets::SocketAddress aSocketAddress)
{
   mTxMsgCount=0;
   mRxMsgCount=0;

   reset();
   mRemote = aSocketAddress;

   if (mMonkey==0) mMonkey = aMonkeyCreator->createMonkey();

   doSocket();
   setOptionKeepAlive();
   setOptionNoDelay();

   if (mStatus==0)
   {
      Prn::print(Prn::SocketInit2, "TcpMsgSocket       $ %16s : %d",
         aSocketAddress.mIpAddr.mString,
         aSocketAddress.mPort);
   }
   else
   {
      Prn::print(Prn::SocketInit2, "TcpMsgSocket       $ %16s : %d $ %d %d",
         aSocketAddress.mIpAddr.mString,
         aSocketAddress.mPort,
         mStatus,
         mError);
   }

   mValidFlag=mStatus==0;
}

void TcpMsgSocket::configure(
   BaseMsgMonkeyCreator* aMonkeyCreator)
{
   if (mMonkey == 0) mMonkey = aMonkeyCreator->createMonkey();
}

//******************************************************************************
// Reconfigure the socket

void TcpMsgSocket::reconfigure()
{
   mTxMsgCount=0;
   mRxMsgCount=0;

   doSocket();
   setOptionKeepAlive();
}

//******************************************************************************
// This copies a message into a byte buffer and then sends the byte buffer 
// out the socket.

bool TcpMsgSocket::doSendMsg(ByteContent* aMsg)
{
   // Guard.
   if (!mValidFlag)
   {
      Prn::print(Prn::SocketRun2, "ERROR doSend when Invalid");
      delete aMsg;
      return false;
   }

   // Create a byte buffer.
   ByteBuffer tBuffer(mMonkey->getMaxBufferSize());

   // Configure the byte buffer.
   mMonkey->configureByteBuffer(&tBuffer);
   tBuffer.setCopyTo();

   // Copy the message to the buffer.
   mMonkey->putMsgToBuffer(&tBuffer,aMsg);

   // Delete the message.
   delete aMsg;

   // Mutex.
   mTxMutex.lock();

   // Transmit the buffer
   bool tRet=false;
   int tLength=tBuffer.getLength();
   tRet = doSend(tBuffer.getBaseAddress(),tLength);
   Prn::print(Prn::SocketRun4, "doSendM %d %d %d",mStatus,mError,tLength);

   mTxMsgCount++;

   // Mutex
   mTxMutex.unlock();

   if (!tRet)
   {
      Prn::print(Prn::SocketRun2, "ERROR TcpMsgSocket::doSendMsg FAIL");
   }

   return true;
}

//******************************************************************************
// This receives data from the socket into a byte buffer and then
// extracts a message from the byte buffer

bool TcpMsgSocket::doReceiveMsg (ByteContent*& aMsg)
{
   //-------------------------------------------------------------------------
   // Initialize
   aMsg=0;
   bool tRet=false;
   int tStatus=0;

   // Create a byte buffer.
   ByteBuffer tBuffer(mMonkey->getMaxBufferSize());

   // Configure the byte buffer.
   mMonkey->configureByteBuffer(&tBuffer);
   tBuffer.setCopyTo();

   //-------------------------------------------------------------------------
   // Read the message header into the receive buffer

   int   tHeaderLength = mMonkey->getHeaderLength();
   char* tHeaderBuffer = tBuffer.getBaseAddress();

   tRet = doRecv(tHeaderBuffer,tHeaderLength,tStatus);
   Prn::print(Prn::SocketRun4, "doRecvH %d %d",mStatus,mError);

   // Guard
   // If bad status then return false.

   if (!tRet || tStatus<=0)
   {
      return false;
   }

   // Set the buffer length
   tBuffer.setLength(tHeaderLength);

   //--------------------------------------------------------------
   // Copy from the receive buffer into the message monkey object
   // and validate the header

   mMonkey->extractMessageHeaderParms(&tBuffer);

   // If the header is not valid then error
   if (!mMonkey->mHeaderValidFlag)
   {
      Prn::print(Prn::SocketRun1, "ERROR doRecv1 INVALID HEADER");
      return false;
   }

   //-------------------------------------------------------------------------
   // Read the message payload into the receive buffer

   int   tPayloadLength = mMonkey->mPayloadLength;
   char* tPayloadBuffer = tBuffer.getBaseAddress() + tHeaderLength;

   tRet=doRecv(tPayloadBuffer,tPayloadLength,tStatus);
   Prn::print(Prn::SocketRun4, "doRecvP %d %d %d",mStatus,mError,tPayloadLength);

   // Guard
   // If bad status then return false.

   if (!tRet || tStatus<=0)
   {
      Prn::print(Prn::SocketRun1, "ERROR doRecv2 INVALID RECV");
      return false;
   }

   // Set the buffer length
   tBuffer.setLength(mMonkey->mMessageLength);

   //--------------------------------------------------------------
   // At this point the buffer contains the complete message.
   // Extract the message from the byte buffer into a new message
   // object and return it.

   tBuffer.rewind();
   aMsg = mMonkey->getMsgFromBuffer(&tBuffer);

   // Test for errors.
   if (aMsg==0)
   {
      Prn::print(Prn::SocketRun1, "ERROR getMsgFromBuffer");
      mStatus=tBuffer.getError();
      return false;
   }

   // Returning true  means socket was not closed
   // Returning false means socket was closed
   mRxMsgCount++;
   return true;
}

}//namespace
}//namespace

