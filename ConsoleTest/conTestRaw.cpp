/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <ctype.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "conTestRaw.h"

namespace Con
{

struct termios gRawOriginalTermios;

void initializeRaw()
{
   tcgetattr(STDIN_FILENO, &gRawOriginalTermios);
   struct termios tNewTermios = gRawOriginalTermios;
   tNewTermios.c_lflag &= ~(ECHO | ICANON | IXON);
   tcsetattr(STDIN_FILENO, TCSAFLUSH, &tNewTermios);
}

void finalizeRaw()
{
   tcsetattr(STDIN_FILENO, TCSAFLUSH, &gRawOriginalTermios);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Raw input and output.

int readOneRaw()
{
   char tChar = 0;
   read(STDIN_FILENO, &tChar, 1);
   return tChar;
}

void writeOneRaw(int aChar)
{
   char tChar = (char)aChar;
   write(STDOUT_FILENO, &tChar, 1);
}

void writeStringRaw(char* aString)
{
   int tLength = (int)strlen(aString);
   write(STDOUT_FILENO, aString, tLength);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Run a test loop that prints raw key codes.

void doTestLoopRaw()
{
   Prn::print(Prn::View11, "doTestLoopRaw****************************");
   while (true)
   {
      int tChar = 0;
      tChar = readOneRaw();
      if (tChar == '1' && false)
      {
         doTest1();
      }
      else
      {
         writeOneRaw(tChar);
         Prn::print(Prn::View11, "char %d", tChar);
      }

      if (tChar == 'z')
      {
         Prn::print(Prn::View11, "exit");
         printf("\nexit\n");
         break;
      }
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Run a test loop that prints raw key codes.

void doTest1()
{
   writeStringRaw("\e[6n");

   char tString[40];
   int tIndex = 0;
   while (true)
   {
      char tChar = readOneRaw();
      tString[tIndex++] = tChar;
      if (tChar == 'R')break;
   }
   tString[tIndex] = 0;

   int tRow = -1;
   int tCol = -1;
   sscanf(tString, "\e[%d;%dR", &tRow, &tCol);

   Prn::print(Prn::View11, "Test1 cursor %4d %4d",tRow,tCol);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
