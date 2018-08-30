#pragma once

/*==============================================================================
This unit provides a function that is similar to printf which can be filtered. 
The way that it is used is that there can be many prints located throughout an
application, with all them being called, but only those ones that are enabled 
actually printing, the ones that are filtered just immediately return.

When a print is executed it is passed a filter index. If the value of the 
filter table at the specified index is enabled then the print is executed.

This unit supports multiple consoles, with the default being one console, the
console application console, with a console index of zero. If more than one
consoles are specified then PrintView console applications are launched and
prints are sent to them vai udp sockets. Console indices for a specific
filter index are stored in the filter table.
==============================================================================*/
//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "prnPrintSettings.h"

namespace Prn
{
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initializes the print facility.
//
// The Reset method is called to reset settings to deafults.
// The Settings methods are called to override defaults.
// The Initialize method does the initialization.
// The Finalize method shuts down any PrintView processes that were launched.
//
// aSettingsFileName is read from the default settings directory. 
// aSettingsFilePath is read from an absolute filepath. 
//
// aSettingsFileSection is the settings file section that each application
// extracts its settings from.
//
// aConsole specifies a print view console to use. If it is greater
// than zero then additional PrintView console applications are launched
// and prints are redirected to them via sockets.

void resetPrint              ();
void useSettingsFileDefault  ();
void useSettingsFileName     (char* aSettingsFileName);
void useSettingsFilePath     (char* aSettingsFilePath);
void useSettingsFileSection  (char* aSettingsFileSection);
void useConsole              (int   aConsole);
void initializePrint         ();
void finalizePrint           ();

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Suppress any prints that do not have a filter of zero.

void suppressPrint       ();
void unsuppressPrint     ();
void toggleSuppressPrint ();

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Filtered print, if the corresponding entry in the filter table is true
// then the print is executed.

void print (int aFilter, const char* aFormat, ...);

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Set a filter in the filter table.
//
// aFilter is the index of the filter.
// aEnablePrint is the value stored in the table at the filter index.
// aConsole is the console index stored in the table at the filter index.

void  setFilter(int aFilter, bool aEnablePrint, int aConsole = 0);

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

