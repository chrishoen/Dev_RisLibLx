//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risCmdLineFile.h"
#include "risAlphaDir.h"
#include "risNetPortDef.h"


#define  _PRNPRINTSETTINGS_CPP_
#include "prnPrintSettings.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Prn
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

PrintSettings::PrintSettings()
{
   strcpy(mPrintViewHostIPAddress, "none");
   mPrintViewHostIPPort = 99;
   mCode1 = 0;
}

void PrintSettings::reset()
{
   char tBuffer[200];
   BaseClass::reset();
   BaseClass::setExplicitFileDir(Ris::getAlphaFilePath_Settings(tBuffer));
   strcpy(BaseClass::mDefaultFileName, "PrnPrint_Settings.txt");

   strcpy(mPrintViewHostIPAddress,"127.0.0.1");
   mPrintViewHostIPPort = Ris::Net::PortDef::cPrintView;

   mCode1 = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show.

void PrintSettings::show()
{
   printf("\n");
   printf("PrintSettings************************************************ %s\n", mTargetSection);

   printf("PrintViewHost           %16s : %5d\n", mPrintViewHostIPAddress, mPrintViewHostIPPort);
   printf("Code1                   %5d\n", mCode1);

   printf("PrintSettings************************************************\n");
   printf("\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Base class override: Execute a command from the command file to set a 
// member variable.  Only process commands for the target section.This is
// called by the associated command file object for each command in the file.

void PrintSettings::execute(Ris::CmdLineCmd* aCmd)
{
   if (!isTargetSection(aCmd)) return;

   if (aCmd->isCmd("PrintViewHostIP"))
   {
      aCmd->copyArgString(1, mPrintViewHostIPAddress, cMaxStringSize);
      mPrintViewHostIPPort = aCmd->argInt(2);
   }

   if (aCmd->isCmd("Code1")) mCode1 = aCmd->argInt(1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Calculate expanded member variables. This is called after the entire
// section of the command file has been processed.

void PrintSettings::expand()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace