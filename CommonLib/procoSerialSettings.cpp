//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risCmdLineFile.h"
#include "risPortableCalls.h"


#define  _PROCOSERIALSETTINGS_CPP_
#include "procoSerialSettings.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace ProtoComm
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

SerialSettings::SerialSettings()
{
   reset();
}

void SerialSettings::reset()
{
   BaseClass::reset();
   strcpy(BaseClass::mDefaultFileName, "ProtoComm_SerialSettings.txt");

   mSerialPortDevice[0] = 0;
   mSerialPortSetup[0] = 0;
   mSerialRxTimeout = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show.

void SerialSettings::show()
{
   printf("\n");
   printf("SerialSettings************************************************ %s\n", mTargetSection);

   printf("SerialPortDevice           %-12s\n", mSerialPortDevice);
   printf("SerialPortSetup            %-12s\n", mSerialPortSetup);
   printf("SerialRxTimeout            %5d\n",   mSerialRxTimeout);

   printf("SerialSettings************************************************\n");
   printf("\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Base class override: Execute a command from the command file to set a 
// member variable.  Only process commands for the target section.This is
// called by the associated command file object for each command in the file.

void SerialSettings::execute(Ris::CmdLineCmd* aCmd)
{
   if (!isTargetSection(aCmd)) return;

   if (aCmd->isCmd("SerialPortDevice"))  aCmd->copyArgString(1, mSerialPortDevice, cMaxStringSize);
   if (aCmd->isCmd("SerialPortSetup"))   aCmd->copyArgString(1, mSerialPortSetup, cMaxStringSize);
   if (aCmd->isCmd("SerialRxTimeout"))   mSerialRxTimeout = aCmd->argInt(1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Calculate expanded member variables. This is called after the entire
// section of the command file has been processed.

void SerialSettings::expand()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace