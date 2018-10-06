/*==============================================================================
Print utility
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <stdarg.h>

#include "risAlphaDir.h"
#include "risPortableCalls.h"
#include "risNetPortDef.h"
#include "risNetUdpStringSocket.h"
#include "risNetSettings.h"

#include "prnPrintSettings.h"
#include "prnPrintFilterTable.h"
#include "prnPrint.h"

namespace Prn
{

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Constants.

static const int cMaxPrintStringSize = 400;
static const int cMaxConsoles = PrintFilterTable::cMaxConsoles;

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Regional variables.

// Suppress any prints that do not have a filter of zero.
bool rSuppressFlag;

// If true then a print view console is used.
bool rConsoleFlag [cMaxConsoles];

// The socket for the print view console.
Ris::Net::UdpTxStringSocket rConsoleSocket[cMaxConsoles];

// The port number for the print view console.
int rConsolePort[cMaxConsoles];

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Reset variables.

void resetVariables()
{
   rSuppressFlag = true;

   for (int i = 0; i < cMaxConsoles; i++)
   {
      rConsoleFlag[i] = false;
      rConsolePort[i] = gPrintSettings.mPrintViewHostIPPort + i;
   }
   // rConsoleFlag[0] = true;
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Reset print facility.

void resetPrint()
{
   // Read from settings file.
   gPrintSettings.reset();
   gPrintSettings.mEnablePrint = false;
   gPrintSettings.readSection("default");

   // Reset variables.
   resetVariables();
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Set console flag. A print view console for the index will be created and
// used.

void useConsole(int aConsole)
{
   if (aConsole > cMaxConsoles-1) return;
   rConsoleFlag[aConsole] = true;
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Initialize print facility.

void initializePrint()
{
   rSuppressFlag = false;

   // For each print view console, create a PrintView process
   // and initialize and configure a socket to send the print to.
   for (int i = 0; i < cMaxConsoles; i++)
   {
      if (rConsoleFlag[i])
      {
         // Create a socket to send to the PrintView console.
         Ris::Net::Settings tSettings;
         tSettings.setRemoteIp(
            gPrintSettings.mPrintViewHostIPAddress,
            rConsolePort[i]);
         rConsoleSocket[i].initialize(tSettings);
         rConsoleSocket[i].configure();
      }
   }
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Finalize the print facility.

void finalizePrint()
{
   // Terminate PrintView processes that were created. 
   for (int i = 1; i < cMaxConsoles; i++)
   {
      rConsoleSocket[i].doClose();
   }
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Set a filter in the filter table.
//
// aFilter is the index of the filter.
// aEnable is the value stored in the table at the filter index.
// aConsole is the console index stored in the table at the filter index.

void setFilter(int aFilter, bool aEnable, int aConsole)
{
   gPrintFilterTable.setFilter(aFilter,aEnable, aConsole);
}   	

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Enable a filter in the filter table.
//
// aFilter is the index of the filter.
// aEnable is the value stored in the table at the filter index.

void enableFilter(int aFilter, bool aEnable)
{
   gPrintFilterTable.enableFilter(aFilter, aEnable);
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Filtered print, if the corresponding entry in the filter table is true
// then the print is executed.

void print(int aFilter, const char* aFormat, ...)
{
   //*************************************************************************
   //*************************************************************************
   //*************************************************************************
   // Exit if print not enabled.

   // Exit if suppressed and the filter is not zero.
   if (rSuppressFlag && aFilter != 0) return;

   // Exit if the filter table entry is disabled.
   if (gPrintFilterTable.mEnable[aFilter] == false) return;

   //*************************************************************************
   //*************************************************************************
   //*************************************************************************
   // Instantiate a print string buffer and a pointer.

   char* tPrintStr = 0;
   char  tPrintBuffer[cMaxPrintStringSize];
   int   tPrintStrSize;
   tPrintStr = &tPrintBuffer[0];

   //*************************************************************************
   //*************************************************************************
   //*************************************************************************
   // Do a vsprintf with variable arg list into print string pointer.

   va_list  ArgPtr;
   va_start(ArgPtr, aFormat);
   tPrintStrSize = vsnprintf(tPrintStr, cMaxPrintStringSize, aFormat, ArgPtr);
   va_end(ArgPtr);

   tPrintStr[tPrintStrSize++] = 0;

   //*************************************************************************
   //*************************************************************************
   //*************************************************************************
   // Print the string.

   // Get the console index assigned to the filter.
   int tConsole = gPrintFilterTable.mConsole[aFilter];

   if (tConsole == 0 && !rConsoleFlag[0])
   {
      // Print to stdout.
      puts(tPrintStr);
   }
   else
   {
      // Print to the PrintView console.
      rConsoleSocket[tConsole].doSendString(tPrintStr);
   }
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Suppress any prints that do not have a filter of zero.

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
// Initialize the regional variables when program is loaded.

class PrintResetClass
{
public:
   PrintResetClass()
   {
      resetVariables();
   }
};

PrintResetClass gPrintResetClass;

//******************************************************************************
//******************************************************************************
//******************************************************************************
} //namespace

