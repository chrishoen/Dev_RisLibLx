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

struct termios gOriginalTermios;

void initializeRaw()
{
   tcgetattr(STDIN_FILENO, &gOriginalTermios);
   struct termios tNewTermios = gOriginalTermios;
   tNewTermios.c_lflag &= ~(ECHO | ICANON);
   tcsetattr(STDIN_FILENO, TCSAFLUSH, &tNewTermios);
}

void finalizeRaw()
{
   tcsetattr(STDIN_FILENO, TCSAFLUSH, &gOriginalTermios);
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
      if (tChar != 'n')
      {
         writeOneRaw(tChar);
      }
      else
      {
         writeOneRaw('\r');
      }

      Prn::print(Prn::View11, "char %d", tChar);
      if (tChar == 'z')
      {
         Prn::print(Prn::View11, "exit");
         break;
      }
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
