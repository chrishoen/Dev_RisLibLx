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
#include <fcntl.h>
#include <sys/ioctl.h>

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
   mIsFunction = false;
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
// Return the number of characters that are available to be read.

int KeyReader::getReadAvailable()
{
   int tBytesAvailable = 0;
   int tRet = ioctl(STDIN_FILENO, FIONREAD, &tBytesAvailable);
   if (tRet < 0)
   {
      printf("KeyReader::getReadAvailable ERROR %d\n", errno);
      return 0;
   }
   return tBytesAvailable;
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
      if (tKeyIn != 27)
      {
         Prn::print(Prn::View24, "READ101 %4d", tKeyIn);
      }

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
   Prn::print(Prn::View24, "READ200*******************");

   // Locals.
   int  tKeyIn2 = 0;
   int  tKeyIn3 = 0;
   int  tKeyIn4 = 0;
   int  tKeyIn5 = 0;
   bool tFound = false;

   // Test for not an escape sequence.
   if (getReadAvailable() == 0)
   {
      // This is not an escape sequence.
      Prn::print(Prn::View24, "READ201   escape");
      aRecord->mCode = cKey_Escape;
      return true;
   }
   Prn::print(Prn::View24, "READ201   27");

   // This is an escape sequence. Read the second key.
   tKeyIn2 = readOne();
   Prn::print(Prn::View24, "READ202 %4d", tKeyIn2);

   // Test if the second key is not 91 .
   if (tKeyIn2 != 91)
   {
      Prn::print(Prn::View24, "READ202 IGNORE %4d", tKeyIn2);
      // Ignore the key.
      return false;
   }

   // The second key is 91. Read the third key.
   tKeyIn3 = readOne();
   Prn::print(Prn::View24, "READ203 %4d", tKeyIn3);

   // Test the third key for an arrow key.
   tFound = false;
   switch (tKeyIn3)
   {
   case  65: aRecord->mCode = cKey_UpArrow;    tFound = true; break;
   case  66: aRecord->mCode = cKey_DownArrow;  tFound = true; break;
   case  67: aRecord->mCode = cKey_RightArrow; tFound = true; break;
   case  68: aRecord->mCode = cKey_LeftArrow;  tFound = true; break;
   }
   if (tFound) return true;

   // Test the third key for 49.
   if (tKeyIn3 == 49)
   {
      // Read the fourth key after the 49
      tKeyIn4 = readOne();
      Prn::print(Prn::View24, "READ204 %4d", tKeyIn4);

      // Test the fourth key after the 49
      if (tKeyIn4 == 126)
      {
         aRecord->mCode = cKey_Home;
         return true;
      }

      // Test the fourth key after the 49
      tFound = false;
      switch (tKeyIn4)
      {
      case  49: aRecord->mCode = cKey_Function;  aRecord->mChar = 1; tFound = true; break;
      case  50: aRecord->mCode = cKey_Function;  aRecord->mChar = 2; tFound = true; break;
      case  51: aRecord->mCode = cKey_Function;  aRecord->mChar = 3; tFound = true; break;
      case  52: aRecord->mCode = cKey_Function;  aRecord->mChar = 4; tFound = true; break;
      case  53: aRecord->mCode = cKey_Function;  aRecord->mChar = 5; tFound = true; break;
      case  55: aRecord->mCode = cKey_Function;  aRecord->mChar = 6; tFound = true; break;
      case  56: aRecord->mCode = cKey_Function;  aRecord->mChar = 7; tFound = true; break;
      case  57: aRecord->mCode = cKey_Function;  aRecord->mChar = 8; tFound = true; break;
      }
      if (!tFound) return false;

      // Read the fifth key.
      tKeyIn5 = readOne();
      Prn::print(Prn::View24, "READ205 %4d", tKeyIn5);
      // Test if the fifth key is 126.
      return tKeyIn5 == 126;
   }

   // Test the third key for 50.
   if (tKeyIn3 == 50)
   {
      // Read the fourth key after the 50.
      tKeyIn4 = readOne();
      Prn::print(Prn::View24, "READ204 %4d", tKeyIn4);

      // Test the fourth key after the 50.
      tFound = false;
      switch (tKeyIn4)
      {
      case  48: aRecord->mCode = cKey_Function;  aRecord->mChar =  9; tFound = true; break;
      case  49: aRecord->mCode = cKey_Function;  aRecord->mChar = 10; tFound = true; break;
      case  51: aRecord->mCode = cKey_Function;  aRecord->mChar = 11; tFound = true; break;
      case  52: aRecord->mCode = cKey_Function;  aRecord->mChar = 12; tFound = true; break;
      }
      if (!tFound) return false;

      // Read the fifth key.
      tKeyIn5 = readOne();
      Prn::print(Prn::View24, "READ205 %4d", tKeyIn5);
      // Test if the fifth key is 126.
      return tKeyIn5 == 126;
   }

   // Test the third key for 51.
   if (tKeyIn3 == 51)
   {
      // Read the fourth key after the 51.
      tKeyIn4 = readOne();
      Prn::print(Prn::View24, "READ204 %4d", tKeyIn4);

      // Test the fourth key after the 51.
      aRecord->mCode = cKey_Delete;
      return tKeyIn4 == 126;
   }

   // Test the third key for 52.
   if (tKeyIn3 == 52)
   {
      // Read the fourth key after the 51.
      tKeyIn4 = readOne();
      Prn::print(Prn::View24, "READ204 %4d", tKeyIn4);

      // Test the fourth key after the 51.
      aRecord->mCode = cKey_End;
      return tKeyIn4 == 126;
   }

   return false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
