/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "risThreadsPriorities.h"
#include "risAlphaDir.h"
#include "tsShare.h"

#define  _TSPRINTTHREAD_CPP_
#include "tsPrintThread.h"

namespace TS
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

PrintThread::PrintThread()
{
   // Set base class thread parameters.
   BaseClass::setThreadName("TSPrint");
   BaseClass::setThreadPrintLevel(TS::PrintLevel(0, 3));
   BaseClass::setThreadPriority(Ris::Threads::gPriorities.mTsPrint);

   // Initialize variables.
   mTerminateFlag = false;
   mFile = 0;
   mWriteCount = 0;
}

PrintThread::~PrintThread()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
void PrintThread::doFileOpenNew()
{
   char tBuf[400];
   char tFileName[40];
   strcpy(tFileName, gShare.mProgramName);
   strcat(tFileName, "Log.txt");

   if (gShare.mProgramLogFilepath[0] == 0)
   {
      mFile = fopen(Ris::getAlphaFilePath_Log(tBuf, tFileName), "w");
   }
   else
   {
      mFile = fopen(gShare.mProgramLogFilepath, "w");
   }
}

void PrintThread::doFileOpenAppend()
{
   char tBuf[400];
   char tFileName[400];
   strcpy(tFileName, gShare.mProgramName);
   strcat(tFileName, "Log.txt");

   if (gShare.mProgramLogFilepath[0] == 0)
   {
      mFile = fopen(Ris::getAlphaFilePath_Log(tBuf, tFileName), "a");
   }
   else
   {
      mFile = fopen(gShare.mProgramLogFilepath, "w");
   }
}

void PrintThread::doFileClose()
{
   if (mFile)
   {
      fclose(mFile);
      mFile = 0;
   }
}

void PrintThread::doFileFlush()
{
   fflush(mFile);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immediately 
// after the thread starts running. This opens the file.

void PrintThread::threadInitFunction()
{
   // Initialize the string queue.
   mStringQueue.initialize(cQueueSize);

   // Open the log file.
   doFileOpenNew();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function, base class overload. This closes the file.

void  PrintThread::threadExitFunction()
{
   // Print any remaining strings. 
   while (true)
   {
      // Try to read a string from the queue.
      if (PrintString* tString = (PrintString*)mStringQueue.tryRead())
      {
         // print the string and then delete it.
         printString(tString);
      }
      else
      {
         break;
      }
   }

   // Disable prints.
   gShare.mPrintEnableFlag = false;

   // Finalize the string queue.
   mStringQueue.finalize();

   // Close the log file.
   doFileClose();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function. This is called by the base class immediately 
// after the thread init function. It runs a loop that waits on the
// semaphore. When it wakes up, it reads a string from the string
// queue and prints it.

void PrintThread::threadRunFunction()
{
   // Loop to wait for posted events and process them.
   int tCount = 0;
   while (true)
   {
      // Wait on the counting semaphore.
      mSemaphore.get();

      // Test for thread termination.
      if (mTerminateFlag) break;

      // Try to read a string from the queue.
      if (PrintString* tString = (PrintString*)mStringQueue.tryRead())
      {
         // print the string and then delete it.
         printString(tString);
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread shutdown function. Set the termination flag, post to the 
// semaphore and wait for the thread to terminate.

void PrintThread::shutdownThread()
{
   // Set the termination flag.
   mTerminateFlag = true;
   // Post to the semaphore.
   mSemaphore.put();
   // Wait for the thread run function to return.
   BaseClass::waitForThreadTerminate();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Print a string to stdout and the program log file and delete it.
// log file and then deletes it. This is called by the thread run function
// when it dequeues a string from the queue.

void PrintThread::printString(PrintString* aString)
{
   // Guard.
   if (!gShare.mPrintEnableFlag)
   {
      delete aString;
      return;
   }

   // Print to stdout.
   if (aString->mOutFlag)
   {
      fputs(aString->mString, stdout);
   }

   // Print to the log file.
   fputs(aString->mString, mFile);
   if (ferror(mFile))
   {
      printf("fputs error1\n");
      perror("fputs error2\n");
   }

   // Done.
   delete aString;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Try to write a string to the to the queue. Return true if
// successful. This is called by print invocations to enqueue a string.
// It writes to the string queue and posts to the semaphore, which
// then wakes up the thread run function to process the string queue.

bool PrintThread::tryWriteString(PrintString* aString)
{
   // Guard.
   if (mTerminateFlag) return false;
   if (!gShare.mPrintEnableFlag) return false;

   // Try to write to the call queue.
   if (!mStringQueue.tryWrite(aString))
   {
      // The write was not successful.
      return false;
   }

   // Post to the semaphore.
   mSemaphore.put();

   // Successful.
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace