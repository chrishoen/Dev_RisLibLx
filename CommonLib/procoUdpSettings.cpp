//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risCmdLineFile.h"
#include "risPortableCalls.h"


#define  _PROCOUDPSETTINGS_CPP_
#include "procoUdpSettings.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace ProtoComm
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

UdpSettings::UdpSettings()
{
   reset();
}

void UdpSettings::reset()
{
   BaseClass::reset();
   strcpy(BaseClass::mDefaultFileName, "ProtoComm_UdpSettings.txt");

   mMyAppNumber = 0;

   mMyUdpIPAddress[0]=0;
   mMyUdpPort = 0;
   mOtherUdpIPAddress[0]=0;
   mOtherUdpPort = 0;

   mThreadTimerPeriod = 0;

   mNumWords = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show.

void UdpSettings::show()
{
   char tBuffer[40];
   printf("\n");
   printf("UdpSettings************************************************ %s\n", mTargetSection);

   printf("MyAppNumber             %16d\n", mMyAppNumber);

   printf("MyUdp                   %16s : %5d\n",mMyUdpIPAddress,mMyUdpPort);
   printf("OtherUdp                %16s : %5d\n",mOtherUdpIPAddress,mOtherUdpPort);

   printf("\n");
   printf("ThreadTimerPeriod       %16d\n", mThreadTimerPeriod);
   printf("PrintLevel              %16s\n", mPrintLevel.asString(tBuffer));
   printf("NumWords                %16d\n", mNumWords);

   printf("UdpSettings************************************************\n");
   printf("\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Base class override: Execute a command from the command file to set a 
// member variable.  Only process commands for the target section.This is
// called by the associated command file object for each command in the file.

void UdpSettings::execute(Ris::CmdLineCmd* aCmd)
{
   if (!isTargetSection(aCmd)) return;

   if (aCmd->isCmd("MyAppNumber"))   mMyAppNumber = aCmd->argInt(1);

   if (aCmd->isCmd("MyUdp"))
   {
      aCmd->copyArgString(1, mMyUdpIPAddress,cMaxStringSize);
      mMyUdpPort = aCmd->argInt(2);
   }

   if (aCmd->isCmd("OtherUdp"))
   {
      aCmd->copyArgString(1, mOtherUdpIPAddress,cMaxStringSize);
      mOtherUdpPort = aCmd->argInt(2);
   }

   if (aCmd->isCmd("ThreadTimerPeriod"))   mThreadTimerPeriod = aCmd->argInt(1);
   if (aCmd->isCmd("PrintLevel"))          mPrintLevel.readArgs(aCmd);
   if (aCmd->isCmd("NumWords"))            mNumWords = aCmd->argInt(1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Calculate expanded member variables. This is called after the entire
// section of the command file has been processed.

void UdpSettings::expand()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace