#pragma once
/*==============================================================================
Description:

Platform Sockets support classes

This is a set of classes that are wrappers for socket calls. They
hide the details of the parameters that are passed to the calls and
of the error codes that are returned. The layout of the classes is
based on the different types of sockets (udp, tcp server, tcp
client). Each socket class has methods that are aliases for
those socket calls that pertain to that type of socket. The socket
calls are not documented here, look in a sockets book or on the web.

Here are the classes:

IPAddress is an ip address.
SocketAddress is an ip address + a port number.

BaseSocket is the socket base class. It contains a local and a host
address. Local is the machine that the socket is running on and Host
is the remote host machine that the socket is connected to.

UdpSocket is a udp datagram socket.

TcpServerHubSocket manages tcp socket session connections. Clients
connect to Hubs and Hubs spawn Nodes to handle the actual data
transfers.

TcpServerNodeSocket does the tcp send/recv transfers with a client.

TcpClientSocket connects to a Hub and send/recv with a node.
   

==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{
namespace Sockets
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

class IpAddress
{
public:

   IpAddress();
   IpAddress(char*     aAddress);
   IpAddress(int       aAddress);

   void reset();

   void set (char*     aAddress);
   void set (int       aAddress);
 
   void setByHostLocal ();
   void setByHostName  (char* aName);

   int  mValue;
   char mString[16];

   void setForBroadcast ();
   bool isBroadcast     ();

   bool isMulticast     ();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

class SocketAddress
{
public:

   SocketAddress();
   SocketAddress(char*     aIpAddr,int aPort);
   SocketAddress(IpAddress aIpAddr,int aPort);
   void reset ();

   void set (char*     aIpAddr,int aPort);
   void set (IpAddress aIpAddr,int aPort);

   IpAddress mIpAddr;
   int       mPort;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

class BaseSocket
{
public:

   BaseSocket();
   virtual ~BaseSocket();
   virtual void reset();
 
   virtual bool doSocket()=0;

   virtual bool doBind();
   virtual bool doClose();

   SocketAddress mLocal;
   SocketAddress mRemote;

   int mType;
   int mProtocol;
 
   bool updateError(int aStatus);

   int mStatus;
   int mError;

   bool setOptionReuseAddr ();
   bool setOptionBroadcast ();
   bool setOptionDontRoute ();
   bool setOptionDontLinger();
   bool setOptionNoDelay();

   bool ioctlFlush         ();
   bool ioctlBlocking      (bool aBlocking);
   bool ioctlGetBcastAddr  (IpAddress& aBcastAddr);

public:
   class BaseSpecific;
   BaseSpecific* mBaseSpecific;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

class BaseUdpSocket : public BaseSocket
{
public:

   virtual bool doSocket    ();
   virtual bool setMulticast();
   virtual bool doConnect   ();
   virtual bool doSend      (char* aPayload,int& aLength);
   virtual bool doRecv      (char* aPayload,int& aLength,int aMaxLength);
   virtual bool doSendTo    (SocketAddress& aHost,char* aPayload,int& aLength);
   virtual bool doRecvFrom  (SocketAddress& aHost,char* aPayload,int& aLength,int aMaxLength);

   bool setOptionMulticast  (IpAddress& aGroup,IpAddress& aInterface);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

class BaseTcpStreamSocket : public BaseSocket
{
public:

   bool doSocket  ();
   bool doConnect ();
   bool doSend    (char* aPayload,int aLength);
   bool doRecv    (char* aPayload,int aLength,int& aStatus);

   bool setOptionKeepAlive();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

class BaseTcpServerHubSocket : public BaseSocket
{
public:
   BaseTcpServerHubSocket();
   virtual ~BaseTcpServerHubSocket();

   bool doSocket ();
   bool doListen ();
   bool doAccept (BaseTcpStreamSocket& aStream);

   void resetReadSet     ();
   void addSelfToReadSet ();
   void addToReadSet     (BaseTcpStreamSocket* aStream);
   bool isSelfInReadSet  ();
   bool isInReadSet      (BaseTcpStreamSocket* aStream);
   int  selectOnReadSet  ();

public:
   class Specific;
   Specific* mSpecific;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

class BaseTcpServerNodeSocket : public BaseTcpStreamSocket
{
public:
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

class BaseTcpClientSocket : public BaseTcpStreamSocket
{
public:
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

char* memAlloc(int aSize);
void  memFree(char* aMem);

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace


