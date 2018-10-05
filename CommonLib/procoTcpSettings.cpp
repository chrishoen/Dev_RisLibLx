//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risCmdLineFile.h"
#include "risPortableCalls.h"


#define  _PROCOTCPSETTINGS_CPP_
#include "procoTcpSettings.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace ProtoComm
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

TcpSettings::TcpSettings()
{
   reset();
}

void TcpSettings::reset()
{
   BaseClass::reset();
   strcpy(BaseClass::mDefaultFileName, "ProtoComm_TcpSettings.txt");

   mMyAppNumber = 0;

   mTcpServerIPAddress[0]=0;
   mTcpServerPort = 0;
   mTcpMaxSessions = 0;

   mThreadTimerPeriod = 0;
   mNumWords = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show.

void TcpSettings::show()
{
   printf("\n");
   printf("TcpSettings************************************************ %s\n", mTargetSection);

   printf("MyAppNumber             %16d\n",       mMyAppNumber);

   printf("TcpServer               %16s : %5d\n", mTcpServerIPAddress, mTcpServerPort);
   printf("TcpMaxSessions          %16d\n",       mTcpMaxSessions);

   printf("\n");
   printf("ThreadTimerPeriod       %16d\n", mThreadTimerPeriod);
   printf("NumWords                %16d\n", mNumWords);

   printf("TcpSettings************************************************\n");
   printf("\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Base class override: Execute a command from the command file to set a 
// member variable.  Only process commands for the target section.This is
// called by the associated command file object for each command in the file.

void TcpSettings::execute(Ris::CmdLineCmd* aCmd)
{
   if (!isTargetSection(aCmd)) return;

   if (aCmd->isCmd("MyAppNumber"))   mMyAppNumber = aCmd->argInt(1);

   if (aCmd->isCmd("TcpServer"))
   {
      aCmd->copyArgString(1, mTcpServerIPAddress,cMaxStringSize);
      mTcpServerPort = aCmd->argInt(2);
   }

   if (aCmd->isCmd("TcpMaxSessions")) mTcpMaxSessions = aCmd->argInt(1);

   if (aCmd->isCmd("ThreadTimerPeriod"))   mThreadTimerPeriod = aCmd->argInt(1);
   if (aCmd->isCmd("NumWords"))            mNumWords = aCmd->argInt(1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Calculate expanded member variables. This is called after the entire
// section of the command file has been processed.

void TcpSettings::expand()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace