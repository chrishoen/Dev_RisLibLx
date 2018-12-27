#pragma once

/*==============================================================================
Byte content message serial port class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <string>
#include <functional>

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

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// This class encapsulates serial port settings. They are used to configure
// the various serial port classes.

class SerialSettings
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // "COM1"
   char mPortDevice[16];

   // "9600,N,8,1". If empty string then use system defaults.
   char mPortSetup[16];

   // Receive timeout. Milliseconds, 0==no timeout
   int mRxTimeout;

   // Thread processor number and priority.
   Ris::Threads::Priority mThreadPriority;

   // Thread print and log levels.
   TS::PrintLevel mPrintLevel;

   // Message monkey creator.
   BaseMsgMonkeyCreator* mMonkeyCreator;

   // Receive byte content message callback qcall.
   Ris::Threads::QCall1<Ris::ByteContent*> mRxMsgQCall;

   // Receive string callback qcall.
   Ris::Threads::QCall1<std::string*> mRxStringQCall;

   // Receive string callback function.
   std::function<void(std::string*)> mRxStringCallback;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   SerialSettings();
   void reset();

   void setPortDevice(char* aPortDevice);
   void setPortSetup(char* aPortSetup);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

