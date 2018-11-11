/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include <time.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>

#include "risPortableCalls.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Regionals

   
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Sleep

void portableSleep(int aTicks)
{
   usleep(aTicks*1000);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
static char rProgramDir[400];

char* portableGetProgramDir()
{
   readlink("/proc/self/exe", rProgramDir, 400);

   bool tGoing = true;
   int tIndex = strlen(rProgramDir) - 1;
   while (tGoing)
   {
      if (rProgramDir[tIndex] == '/')
      {
         rProgramDir[tIndex + 1] = 0;
         tGoing = false;
      }
      if (--tIndex == 0) tGoing = false;
   }

   return rProgramDir;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
static char rCurrentDir[400];

char* portableGetCurrentDir()
{
   getcwd(rCurrentDir,400);
   strcat(rCurrentDir, "/");
   return rCurrentDir;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void portableWaitForKbhit()
{
   // Locals
   struct termios tAttribSave;
   struct termios tAttribNew;
   fd_set tReadFs;

   // Save current attributes
   tcgetattr( STDIN_FILENO, &tAttribSave);
   // Set new attributes
   tAttribNew = tAttribSave;
   tAttribNew.c_lflag &= ~( ICANON );
   tcsetattr( STDIN_FILENO, TCSANOW, &tAttribNew);
   // Wait for keypressed
   FD_ZERO(&tReadFs);
   FD_SET (STDIN_FILENO, &tReadFs);
   select(STDIN_FILENO+1, &tReadFs, NULL, NULL, NULL);
   // Restore saved attributes
   tcsetattr( STDIN_FILENO, TCSANOW, &tAttribSave);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool portableKbhit()
{
   // Locals
   struct termios tAttribSave;
   struct termios tAttribNew;
   fd_set         tReadFs;
   struct timeval tTimeval;
   
   // Save current attributes
   tcgetattr( STDIN_FILENO, &tAttribSave);
   // Set new attributes
   tAttribNew = tAttribSave;
   tAttribNew.c_lflag &= ~( ICANON );
 //tcsetattr( STDIN_FILENO, TCSANOW, &tAttribNew);
   // Wait for keypressed
   FD_ZERO(&tReadFs);
   FD_SET (STDIN_FILENO, &tReadFs);
   tTimeval.tv_sec=0;
   tTimeval.tv_usec=0;
   int tStatus=select(STDIN_FILENO+1, &tReadFs, NULL, NULL, &tTimeval);
   // Restore saved attributes
 //tcsetattr( STDIN_FILENO, TCSANOW, &tAttribSave);

   if (tStatus != 0)
   {
      getc(stdin);
      return true;
   }

   return false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

long long int portableGetHiResCounter()
{
   struct timespec tTimespec;
   clock_gettime(CLOCK_MONOTONIC, &tTimespec);

   unsigned long long int tSeconds = (unsigned long long int)tTimespec.tv_sec;
   unsigned long long int tNanoseconds = (unsigned long long int)tTimespec.tv_nsec;
   long long int tNanosecondsPerSecond = 1000000000;
   long long int tTimeNs = tSeconds * tNanosecondsPerSecond + tNanoseconds;
   return tTimeNs;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

long long int portableGetHiResFrequency()
{
   return 1000 * 1000 * 1000;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Convert string to upper case

void portableStrupr(char* aString)
{
   unsigned char* tPtr = (unsigned char*)aString;
   int tIndex = 0;
   while (tPtr[tIndex] != 0)
   {
      int tValue = (int)tPtr[tIndex];
      if ((tValue >= 97) && (tValue <= 122))
      {
         tValue -= 32;
         tPtr[tIndex] = (unsigned char)tValue;
      }
      tIndex++;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Stub

void portableSetConsoleTitle(char* aTitle)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool portableFilePathExists(char* aFilePath)
{
   struct stat tStat;
   return (stat(aFilePath, &tStat) == 0);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Set the program current working directory.

void portableChdir(const char* aFilePath)
{
   int tRet = chdir(aFilePath);
   if (tRet) printf("portableChdir FAIL %s", aFilePath);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

