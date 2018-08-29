/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/time.h>

#include "risPortableCalls.h"

namespace Ris
{

//******************************************************************************
// Regionals

static bool rAbortKeyPressed=false;

//******************************************************************************
// Sleep

void portableSleep(int aTicks)
{
   usleep(aTicks*1000);
}

//******************************************************************************
// Settings directory

const char* portableGetSettingsDir()
{
   return "/home/Prime/settings/";
// return "/ide/settings/";
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// System shutdown

void portableShutdown()
{
   printf("SYSTEM SHUTDOWN BEGIN\n");
   system("sudo shutdown -h now");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Convert string to upper case

void portableStrupr(char* aString)
{
   unsigned char* tPtr = (unsigned char*)aString;
   int tIndex=0;
   while (tPtr[tIndex]!=0)
   {
      int tValue = (int)tPtr[tIndex];
      if ((tValue >= 97)&&(tValue <= 122))
      {
         tValue -= 32;
         tPtr[tIndex] = (unsigned char)tValue;
      }
      tIndex++;
   }
}

//******************************************************************************
// Stub

void portableSetConsoleTitle(char* aTitle)
{
}

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
void portableAbortWaitForKbhit()
{
   rAbortKeyPressed=true;
   ungetc('\n',stdin);
}

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
long long int portableGetHiResCounter()
{
   struct timespec tTimespec;
   clock_gettime(CLOCK_MONOTONIC,&tTimespec);

   unsigned long long int tSeconds     = (unsigned long long int)tTimespec.tv_sec;
   unsigned long long int tNanoseconds = (unsigned long long int)tTimespec.tv_nsec;
   long long int tNanosecondsPerSecond = 1000000000;
   long long int tTimeNs = tSeconds*tNanosecondsPerSecond + tNanoseconds;
   return tTimeNs;
}

//******************************************************************************
long long int portableGetHiResFrequency()
{
   return 1000*1000*1000;
}

}//namespace

#if 0
/*==============================================================================
Copyright 2013 Christopher Hoen

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
#endif
