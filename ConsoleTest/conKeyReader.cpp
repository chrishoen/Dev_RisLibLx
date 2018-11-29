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
   mIsControl = false;
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

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read an input key.

   while (true)
   {
      // Read the input key.
      int tKeyIn = readOne();
      Prn::print(Prn::View14, "READ101 %4d", tKeyIn);

      // Test the input for end of read.
      if (tKeyIn == 'z')
      {
         onKey_EndOfRead(tKeyIn,aRecord);
         return;
      }

      // Test the input for back space.
      if (tKeyIn == 8)
      {
         onKey_BackSpace(tKeyIn, aRecord);
         return;
      }

      // Test the input for enter.
      if (tKeyIn == 10)
      {
         onKey_Enter(tKeyIn, aRecord);
         return;
      }

      // Test the input for printable.
      if (isprint(tKeyIn))
      {
         onKey_Printable(tKeyIn, aRecord);
         return;
      }

      // Test the input for control.
      if (1 <= tKeyIn && tKeyIn <= 26)
      {
         onKey_Control(tKeyIn, aRecord);
         return;
      }

      // Test the input for escape.
      if (tKeyIn == 27)
      {
         if (onKey_Escape(tKeyIn, aRecord)) return;
      }

      //***************************************************************************
      //***************************************************************************
      //***************************************************************************
      // The first key was Escape. Read again and test.

   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void KeyReader::onKey_EndOfRead(int aKeyIn, KeyRecord* aRecord)
{
   aRecord->mIsEndOfRead = true;
   aRecord->mCode = cKey_EndOfRead;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void KeyReader::onKey_Enter(int aKeyIn, KeyRecord* aRecord)
{
   aRecord->mCode = cKey_Enter;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void KeyReader::onKey_BackSpace(int aKeyIn, KeyRecord* aRecord)
{
   aRecord->mCode = cKey_BackSpace;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void KeyReader::onKey_Printable(int aKeyIn, KeyRecord* aRecord)
{
   aRecord->mIsPrintable = true;
   aRecord->mCode = cKey_Printable;
   aRecord->mChar = aKeyIn;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void KeyReader::onKey_Control(int aKeyIn, KeyRecord* aRecord)
{
   aRecord->mIsControl = true;
   aRecord->mCode = cKey_Control;
   aRecord->mChar = 96 + aKeyIn;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool KeyReader::onKey_Escape(int aKeyIn, KeyRecord* aRecord)
{
   // Locals.
   int  tKeyIn = 0;
   bool tFound = false;

   // Read the second key.
   tKeyIn = readOne();
   Prn::print(Prn::View14, "READ201 %4d", tKeyIn);

   // Test if the second key is not 91 .
   if (tKeyIn != 91)
   {
      Prn::print(Prn::View14, "READ202 IGNORE %4d", tKeyIn);
      // Ignore the key.
      return false;
   }

   // The second key is 91. Read the third key.
   tKeyIn = readOne();
   Prn::print(Prn::View14, "READ203 %4d", tKeyIn);

   // Test the third key for an arrow key.
   tFound = false;
   switch (tKeyIn)
   {
   case  65: aRecord->mCode = cKey_UpArrow;    tFound = true; break;
   case  66: aRecord->mCode = cKey_DownArrow;  tFound = true; break;
   case  67: aRecord->mCode = cKey_RightArrow; tFound = true; break;
   case  68: aRecord->mCode = cKey_LeftArrow;  tFound = true; break;
   }
   if (tFound) return true;

   // Test the third key for other keys.
   tFound = false;
   switch (tKeyIn)
   {
   case  49: aRecord->mCode = cKey_Home;   tFound = true; break;
   case  52: aRecord->mCode = cKey_End;    tFound = true; break;
   case  51: aRecord->mCode = cKey_Delete; tFound = true; break;
   }
   if (!tFound) return false;

   // Read the fourth key.
   tKeyIn = readOne();
   Prn::print(Prn::View14, "READ204 %4d", tKeyIn);

   // Test if the fourth key is 126.
   if (tKeyIn == 126) return true;

   // The third key was not 126.
   Prn::print(Prn::View11, "**************** READ ERROR 101 %4d", tKeyIn);
   return false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
