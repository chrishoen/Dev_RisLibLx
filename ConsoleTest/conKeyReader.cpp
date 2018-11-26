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

#define  _CONKEYREADER_CPP_
#include "conKeyReader.h"

namespace Con
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

KeyRecord::KeyRecord()
{
   reset();
}

void KeyRecord::reset()
{
   mCode = 0;
   mChar = 0;
   mIsPrintable = false;
   mIsEndOfRead = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

KeyReader::KeyReader()
{
}

struct termios gKeyReaderOriginalTermios;

void KeyReader::initialize()
{
   tcgetattr(STDIN_FILENO, &gKeyReaderOriginalTermios);
   struct termios tNewTermios = gKeyReaderOriginalTermios;
   tNewTermios.c_lflag &= ~(ECHO | ICANON | IXON);
   tcsetattr(STDIN_FILENO, TCSAFLUSH, &tNewTermios);
}

void KeyReader::finalize()
{
   tcsetattr(STDIN_FILENO, TCSAFLUSH, &gKeyReaderOriginalTermios);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Read a single character from the console.

int KeyReader::readOne()
{
   char tChar = 0;
   read(STDIN_FILENO, &tChar, 1);
   return tChar;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Write one char.

void KeyReader::writeOne(char aChar)
{
   char tChar = (char)aChar;
   write(STDOUT_FILENO, &tChar, 1);
}

// Write a string to the console.
void KeyReader::writeString(char* aString)
{
   int tLength = (int)strlen(aString);
   write(STDOUT_FILENO, aString, tLength);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Read a single console keyboard input. Return it in the input record.

void KeyReader::readKey(KeyRecord* aRecord)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Do this first.

   // Clear the input record.
   aRecord->reset();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Local variables.

   int tKeyIn = 0;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read and test.

   // Read the input key.
   tKeyIn = readOne();
   Prn::print(Prn::View14, "READ101 %4d", tKeyIn);

   // Test the input for end of read.
   if (tKeyIn == 'z')
   {
      aRecord->mIsEndOfRead = true;
      aRecord->mCode = cKey_EndOfRead;
   }

   // Test the input for enter.
   if (tKeyIn == 13)
   {
      aRecord->mCode = cKey_Enter;
      return;
   }

   // Test the input for enter.
   if (tKeyIn == 'S')
   {
      aRecord->mCode = cKey_Special;
      return;
   }

   // Test the input for back space.
   if (tKeyIn == 8)
   {
      aRecord->mCode = cKey_BackSpace;
      return;
   }

   // Test the input for enter.
   if (tKeyIn == 10)
   {
      aRecord->mCode = cKey_Enter;
      return;
   }

   // Test the input for not special.
   if (tKeyIn != 27)
   {
      // Test the input for printable.
      if (isprint(tKeyIn))
      {
         aRecord->mIsPrintable = true;
         aRecord->mCode = cKey_Printable;
         aRecord->mChar = tKeyIn;
         return;
      }
      // Not special and not printable.
      else
      {
         aRecord->mCode = cKey_Ignore;
         return;
      }
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read again and test.

   // Read the second key.
   tKeyIn = readOne();
   Prn::print(Prn::View14, "READ102 %4d", tKeyIn);

   // Test if second key not 91 .
   if (tKeyIn != 91)
   {
      aRecord->mCode = cKey_Ignore;
      return;
   }

   // Second key is 91. Read the third key.
   tKeyIn = readOne();
   Prn::print(Prn::View14, "READ103 %4d", tKeyIn);

   // Test the third key for arrow key.
   switch (tKeyIn)
   {
   case  65: aRecord->mCode = cKey_UpArrow; return;
   case  66: aRecord->mCode = cKey_DownArrow; return;
   case  67: aRecord->mCode = cKey_RightArrow; return;
   case  68: aRecord->mCode = cKey_LeftArrow; return;
   }

   // Test the third key for other keys.
   switch (tKeyIn)
   {
   case  49: aRecord->mCode = cKey_Home; break;
   case  52: aRecord->mCode = cKey_End; break;
   case  51: aRecord->mCode = cKey_Delete; break;
   default : aRecord->mCode = cKey_Ignore; return;
   }

   // Read the fourth key.
   tKeyIn = readOne();
   Prn::print(Prn::View14, "READ104 %4d", tKeyIn);

   // Test if the fourth key is 126.
   if (tKeyIn == 126)
   {
      return;
   }

   // The third key was not 126.
   aRecord->mCode = cKey_Ignore;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
