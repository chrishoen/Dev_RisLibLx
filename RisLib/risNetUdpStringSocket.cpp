/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "prnPrint.h"

#include "risNetUdpStringSocket.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

UdpRxStringSocket::UdpRxStringSocket()
{
   mRxString[0]=0;
   mRxLength=0;
   mRxCount=0;
   mValidFlag=false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// configure the socket

void UdpRxStringSocket::configure(int aPort)
{
   Sockets::SocketAddress tLocal;
   tLocal.set("127.0.0.1",aPort);
   configure(tLocal);
}

void UdpRxStringSocket::configure(Sockets::SocketAddress aLocal)
{
   mRxCount=0;

   mLocal = aLocal;

   doSocket();
   doBind();

   if (mStatus==0)
   {
      Prn::print(Prn::SocketInit2, "UdpRxStringSocket     $ %16s : %d",
         aLocal.mIpAddr.mString,
         aLocal.mPort);
   }
   else
   {
      Prn::print(Prn::SocketInit2, "UdpRxStringSocket     $ %16s : %d $ %d %d",
         aLocal.mIpAddr.mString,
         aLocal.mPort,
         mStatus,
         mError);
   }

   mValidFlag=mStatus==0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This receives a datagram from the socket into a byte buffer and then
// extracts a message from the byte buffer

bool UdpRxStringSocket::doRecvString ()
{
   //-------------------------------------------------------------------------
   // Initialize
   mRxString[0]=0;
   mRxLength=0;

   // Guard
   if (!mValidFlag) return false;

   //-------------------------------------------------------------------------
   // Read the message into the receive buffer
   
   doRecvFrom (mFromAddress,mRxString,mRxLength,cStringSize);

   // Guard
   // If bad status then return false.
   // Returning true  means socket was not closed
   // Returning false means socket was closed

   if (mRxLength<=0)
   {
      if (mStatus<0)
         switch (mError)
         {
            case 0              : return false  ;break;
            default             : return false  ;break;
         }   
      else
         return false;
   }

   // Null terminator
   mRxString[mRxLength] = 0;

   // Returning true  means socket was not closed
   // Returning false means socket was closed
   mRxCount++;
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

UdpTxStringSocket::UdpTxStringSocket()
{
   mTxLength=0;
   mTxCount=0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Configure the socket. Use with the next doSendMsg.

void UdpTxStringSocket::configure(int aPort)
{
   Sockets::SocketAddress tLocal;
   tLocal.set("127.0.0.1",aPort);
   configure(tLocal);
}

void UdpTxStringSocket::configure(Sockets::SocketAddress aRemote)
{
   mTxCount=0;

   mRemote = aRemote;

   doSocket();

   if (mStatus==0)
   {
      Prn::print(Prn::SocketInit2, "UdpTxStringSocket     $ %16s : %d",
         aRemote.mIpAddr.mString,
         aRemote.mPort);
   }
   else
   {
      Prn::print(Prn::SocketInit2, "UdpTxStringSocket     $ %16s : %d $ %d %d",
         aRemote.mIpAddr.mString,
         aRemote.mPort,
         mStatus,
         mError);
   }

   mValidFlag=mStatus==0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This copies a message into a byte buffer and then sends the byte buffer 
// out the socket. Use with the previous configure.

bool UdpTxStringSocket::doSendString(char* aString)
{
   // Guard
   if (!mValidFlag) return false;

   // Mutex
   mTxMutex.lock();

   // Transmit the buffer
   int tLength = (int)strlen(aString);
   doSendTo(mRemote,aString,tLength);

   mTxLength = tLength;
   mTxCount++;

   // Mutex
   mTxMutex.unlock();

   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

