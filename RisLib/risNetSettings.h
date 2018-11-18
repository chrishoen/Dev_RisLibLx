#pragma once

/*==============================================================================
Byte content message network socket settings class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risByteContent.h"
#include "risByteMsgMonkey.h"
#include "risThreadsQCall.h"
#include "tsDefs.h"
#include "risThreadsPriorities.h"

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
// This class encapsulates network socket settings. They are used to configure
// the various socket classes.

class Settings
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // IP address.
   char mLocalIpAddr[20];

   // IP port.
   int mLocalIpPort;

   // IP address.
   char mRemoteIpAddr[20];

   // IP port.
   int mRemoteIpPort;

   // Max number of tcp server sessions.
   int mMaxSessions;

   // Socket flags.
   int mFlags;

   // Thread processor number and priority.
   Ris::Threads::Priority mThreadPriority;

   // Thread print and log levels.
   TS::PrintLevel mPrintLevel;

   // Message monkey creator.
   BaseMsgMonkeyCreator* mMonkeyCreator;

   // Session callback qcall.
   Ris::Threads::QCall1<bool>     mClientSessionQCall;
   Ris::Threads::QCall2<int,bool> mServerSessionQCall;

   // Receive byte content message callback qcall.
   Ris::Threads::QCall1<Ris::ByteContent*> mRxMsgQCall;
   Ris::Threads::QCall2<int,Ris::ByteContent*> mServerRxMsgQCall;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   Settings();

   // Set member.
   void setLocalIp (char* aIpAddr, int aIpPort);
   void setRemoteIp(char* aIpAddr, int aIpPort);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

