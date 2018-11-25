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

#define  _CONCONSOLE_CPP_
#include "conConsole.h"

namespace Con
{

struct termios gOriginalTermios22;

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

Console::Console()
{
}

Console::~Console()
{
}

// Initialize the console for raw input.
void Console::initialize()
{
   tcgetattr(STDIN_FILENO, &gOriginalTermios22);
   struct termios tNewTermios = gOriginalTermios22;
   tNewTermios.c_lflag &= ~(ECHO | ICANON);
   tcsetattr(STDIN_FILENO, TCSAFLUSH, &tNewTermios);
}

// Restore the console from raw input.
void Console::finalize()
{
   tcsetattr(STDIN_FILENO, TCSAFLUSH, &gOriginalTermios22);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Run test loop.

void Console::doTestLoop1()
{
   initialize();
   int tCount = 0;
   Prn::print(Prn::View11, "doTestLoop1****************************");
   while (true)
   {
      char tChar = 0;
      read(STDIN_FILENO, &tChar, 1);

      if (tChar != 'n')
      {
         //write(STDIN_FILENO, &tChar, 1);
      }
      else
      {
         char tTempChar = 'H';
         //write(STDIN_FILENO, &tTempChar, 1);
      }


      Prn::print(Prn::View11, "%4d char %d", tCount++, tChar);
      if (tChar == 'z')
      {
         Prn::print(Prn::View11, "escape");
         break;
      }
   }

   finalize();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Run test loop.

void Console::doTestLoop2()
{
   Prn::print(Prn::View11, "doTestLoop2****************************");
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
