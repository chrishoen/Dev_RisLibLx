/*==============================================================================
Print utility
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "risPortableCalls.h"
#include "risNetPortDef.h"
#include "risNetUdpStringSocket.h"

#include "prnPrint.h"

namespace Prn
{

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Regional variables

   static const int cMaxPrintStringSize = 400;
   static const int cMaxNameSize = 200;
   static const int cMaxConsoles = 5;

   bool    rUseSettingsFile;
   char    rSettingsFilePath    [cMaxNameSize];
   char    rSettingsFileSection [cMaxNameSize];
   int     rNumOfConsoles;
   bool    rSuppressFlag;

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Initialize settings to defaults

void resetPrint()
{
   rUseSettingsFile = false;
   rSettingsFilePath[0]=0;
   strcpy(rSettingsFileSection, "DEFAULT");
   rNumOfConsoles=1;
   rSuppressFlag=true;

   strncpy(rSettingsFilePath,Ris::portableGetSettingsDir(),cMaxNameSize);
   strncat(rSettingsFilePath,"prnPrintSettings.txt",cMaxNameSize);
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Override defaults

void useSettingsFileDefault()
{
   strncpy(rSettingsFilePath,Ris::portableGetSettingsDir(),cMaxNameSize);
   strncat(rSettingsFilePath,"prnPrintSettings.txt",cMaxNameSize);
   rUseSettingsFile=true;
}

void useSettingsFileName(char* aSettingsFileName)
{
   strncpy(rSettingsFilePath,Ris::portableGetSettingsDir(),cMaxNameSize);
   strncat(rSettingsFilePath,aSettingsFileName,cMaxNameSize);
   rUseSettingsFile=true;
}

void useSettingsFilePath(char* aSettingsFilePath)
{
   strncpy(rSettingsFilePath,aSettingsFilePath,cMaxNameSize);
   rUseSettingsFile=true;
}

void useSettingsFileSection(char*aSettingsFileSection)
{
   strncpy(rSettingsFileSection, aSettingsFileSection, cMaxNameSize);
}

void useConsoles(int aNumOfConsoles)
{
   rNumOfConsoles = aNumOfConsoles;
   if (rNumOfConsoles > cMaxConsoles) rNumOfConsoles = cMaxConsoles;
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Initialize

void initializePrint()
{
   //-----------------------------------------------------
   // Settings

   if (rUseSettingsFile)
   {
      gSettings.initialize(rSettingsFilePath, rSettingsFileSection);
   }

   //-----------------------------------------------------
   // Regionals

   rSuppressFlag = false;
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Finalize
void finalizePrint()
{
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Set filter table entry

void setFilter(int aFilter, bool aEnablePrint, int aConsole)
{
   gSettings.setFilter(aFilter,aEnablePrint, aConsole);
}   	

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Print

void print(int aFilter, const char* aFormat, ...)
{
   //-----------------------------------------------------
   // Guard for print not enabled

   // Test filter table entry
   if (gSettings.mFilterTable[aFilter] == false) return;

   // Get the console index assigned to the filter
   int tConsole = gSettings.mConsoleTable[aFilter];

   // If suppressed and the filter is not zero and
   // the console is zero then exit
   if (rSuppressFlag && aFilter != 0 && tConsole == 0) return;

   //-----------------------------------------------------
   // Print string pointer

   char* tPrintStr = 0;
   char  tPrintBuffer[cMaxPrintStringSize];
   int   tPrintStrSize;
   tPrintStr = &tPrintBuffer[0];

   //-----------------------------------------------------
   // Do a vsprintf with variable arg list into print string pointer

   va_list  ArgPtr;
   va_start(ArgPtr, aFormat);
   tPrintStrSize = vsnprintf(tPrintStr, cMaxPrintStringSize, aFormat, ArgPtr);
   va_end(ArgPtr);

   tPrintStr[tPrintStrSize++] = 0;

   //-----------------------------------------------------
   // Print the string

   puts(tPrintStr);
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Suppress

void suppressPrint()
{
   rSuppressFlag=true;
}

void unsuppressPrint()
{
   rSuppressFlag=false;
}

void toggleSuppressPrint()
{
   rSuppressFlag = !rSuppressFlag;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize the regional variables when program is loaded

class PrintResetClass
{
public:
   PrintResetClass()
   {
      resetPrint();
   }
};

PrintResetClass gPrintResetClass;

} //namespace

