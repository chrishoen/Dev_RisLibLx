#pragma once

/*==============================================================================
Implements multiple text log files
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Log
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initializes the log files facility.

   void reset    ();
   bool openFile       (int aLogNum,char* aFileName);
   bool openFileAppend (int aLogNum,char* aFileName);

   bool openFile       (int aLogNum,char* aFileDir,char* aFileName);
   bool openFileAppend (int aLogNum,char* aFileDir,char* aFileName);

   void closeFile      (int aLogNum);
   void closeAllFiles ();

   void enableTimeStamp(int aLogNum,bool aEnableFlag);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Write to log file

   void write (int aLogNum, const char* aFormat, ...);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Write to log file

   void writeTimeStamp (int aLogNum, const char* aLabel);

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

