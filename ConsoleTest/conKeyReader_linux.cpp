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
#include "conKeyReader_linux.h"

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
   mIsShift = false;
   mIsAlt = false;
   mIsControl = false;
   mIsControlShift = false;
   mIsAltShift = false;
   mIsFunction = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

KeyReader::KeyReader()
{
   mPF = true;;
}

struct termios gKeyReaderOriginalTermios;

void KeyReader::initialize()
{
   tcgetattr(STDIN_FILENO, &gKeyReaderOriginalTermios);
   struct termios tNewTermios = gKeyReaderOriginalTermios;
// tNewTermios.c_lflag &= ~(ECHO | ICANON | IXON | ISIG | IEXTEN);
   cfmakeraw(&tNewTermios);
   tNewTermios.c_oflag = gKeyReaderOriginalTermios.c_oflag;
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
   // Read an input key.

   while (true)
   {
      // Read the input key.
      int tKeyIn = readOne();

      // Test the input for control c.
      if (tKeyIn == 3)
      {
         onKey_ControlC(tKeyIn, aRecord);
         return;
      }

      if (tKeyIn != 27)
      {
         if(mPF) Prn::print(Prn::View24, "READ101 %4d", tKeyIn);
      }

      // Test the input for back space.
      if (tKeyIn == 8)
      {
         onKey_BackSpace(tKeyIn, aRecord);
         return;
      }

      // Test the input for enter.
//    if (tKeyIn == 10)
      if (tKeyIn == 13)
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
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void KeyReader::onKey_ControlC(int aKeyIn, KeyRecord* aRecord)
{
   printf("\nexiting program from control-c\n");
   finalize();
   exit(1);
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
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Do this first.

   if (mPF) Prn::print(Prn::View24, "READ200*******************");

   // Byte count. 1..7. 1 is a single escape. More than 1 is an escape
   // sequence.
   int  tCount = 0;

   // Byte sequence. 0 is not used. 1 is 27. 2 should be 91.
   int  tB[10];

   // Flag.
   bool tFound = false;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Test for not an escape sequence, for a single escape.

   tCount = getReadAvailable() + 1;

   if (tCount == 1)
   {
      // This is not an escape sequence.
      if (mPF) Prn::print(Prn::View24, "READ201   escape");
      aRecord->mCode = cKey_Escape;
      return true;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // This is an escape sequence. Read all of the available bytes.

   if (mPF) Prn::print(Prn::View24, "READ200 %4d", tCount);

   // Read the available bytes that are after the 27.
   tB[0] = 0;  // Not used.
   tB[1] = 27; // Escape.
   for (int i = 2; i <= tCount; i++)
   {
      tB[i] = readOne();
   }

   // Guard. This is done after reading the available bytes,
   // so that the bytes are read from the buffer.
   if (tCount > 7)
   {
      printf("ESCAPE ERROR bad count %d\n", tCount);
      return false;
   }

   if (tCount >= 1) if (mPF) Prn::print(Prn::View24, "READ201 %4d", tB[1]);
   if (tCount >= 2) if (mPF) Prn::print(Prn::View24, "READ202 %4d", tB[2]);
   if (tCount >= 3) if (mPF) Prn::print(Prn::View24, "READ203 %4d", tB[3]);
   if (tCount >= 4) if (mPF) Prn::print(Prn::View24, "READ204 %4d", tB[4]);
   if (tCount >= 5) if (mPF) Prn::print(Prn::View24, "READ205 %4d", tB[5]);
   if (tCount >= 6) if (mPF) Prn::print(Prn::View24, "READ206 %4d", tB[6]);
   if (tCount >= 7) if (mPF) Prn::print(Prn::View24, "READ207 %4d", tB[7]);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Test for sequences of length 2.

   if (tCount == 2)
   {
      // Store the bytes.
      aRecord->mCode = cKey_Alt;
      aRecord->mChar = tB[2];

      // Done.
      return true;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Test for sequences of length 3.

   // Guard.
   if (tB[2] != 91)
   {
      printf("ESCAPE ERROR not 91 %d\n", tB[2]);
      return false;
   }

   if (tCount == 3)
   {
      // Test the bytes.
      tFound = false;
      if (tB[3] == 65) { aRecord->mCode = cKey_UpArrow; tFound = true; }
      if (tB[3] == 66) { aRecord->mCode = cKey_DownArrow; tFound = true; }
      if (tB[3] == 67) { aRecord->mCode = cKey_RightArrow; tFound = true; }
      if (tB[3] == 68) { aRecord->mCode = cKey_LeftArrow; tFound = true; }

      // Guard.
      if (!tFound)
      {
         printf("ESCAPE ERROR 302 %d\n", tB[3]);
         return false;
      }

      // Done.
      return true;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Test for sequences of length 4.

   if (tCount == 4)
   {
      // Guard.
      if (tB[4] != 126)
      {
         printf("ESCAPE ERROR 401 %d\n", tB[4]);
         return false;
      }

      // Test the bytes.
      tFound = false;
      if (tB[3] == 49) { aRecord->mCode = cKey_Home; tFound = true; }
      if (tB[3] == 50) { aRecord->mCode = cKey_Insert; tFound = true; }
      if (tB[3] == 51) { aRecord->mCode = cKey_Delete; tFound = true; }
      if (tB[3] == 52) { aRecord->mCode = cKey_End; tFound = true; }
      if (tB[3] == 53) { aRecord->mCode = cKey_PageUp; tFound = true; }
      if (tB[3] == 54) { aRecord->mCode = cKey_PageDown; tFound = true; }

      // Guard.
      if (!tFound)
      {
         printf("ESCAPE ERROR 402 %d\n", tB[3]);
         return false;
      }

      // Done.
      return true;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Test for sequences of length 5.

   if (tCount == 5)
   {
      // Guard.
      if (tB[5] != 126)
      {
         printf("ESCAPE ERROR 501 %d\n", tB[5]);
         return false;
      }

      // Test the bytes.
      tFound = false;
      if (tB[3] == 49 && tB[4] == 49) { aRecord->mCode = cKey_Function;  aRecord->mChar = 1; tFound = true; }
      if (tB[3] == 49 && tB[4] == 50) { aRecord->mCode = cKey_Function;  aRecord->mChar = 2; tFound = true; }
      if (tB[3] == 49 && tB[4] == 51) { aRecord->mCode = cKey_Function;  aRecord->mChar = 3; tFound = true; }
      if (tB[3] == 49 && tB[4] == 52) { aRecord->mCode = cKey_Function;  aRecord->mChar = 4; tFound = true; }
      if (tB[3] == 49 && tB[4] == 53) { aRecord->mCode = cKey_Function;  aRecord->mChar = 5; tFound = true; }
      if (tB[3] == 49 && tB[4] == 55) { aRecord->mCode = cKey_Function;  aRecord->mChar = 6; tFound = true; }
      if (tB[3] == 49 && tB[4] == 56) { aRecord->mCode = cKey_Function;  aRecord->mChar = 7; tFound = true; }
      if (tB[3] == 49 && tB[4] == 57) { aRecord->mCode = cKey_Function;  aRecord->mChar = 8; tFound = true; }
      if (tB[3] == 50 && tB[4] == 48) { aRecord->mCode = cKey_Function;  aRecord->mChar = 9; tFound = true; }
      if (tB[3] == 50 && tB[4] == 49) { aRecord->mCode = cKey_Function;  aRecord->mChar = 10; tFound = true; }
      if (tB[3] == 50 && tB[4] == 51) { aRecord->mCode = cKey_Function;  aRecord->mChar = 11; tFound = true; }
      if (tB[3] == 50 && tB[4] == 52) { aRecord->mCode = cKey_Function;  aRecord->mChar = 12; tFound = true; }

      // Guard.
      if (!tFound)
      {
         printf("ESCAPE ERROR 502\n");
         return false;
      }

      // Done.
      return true;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Test for count is 7.

   if (tCount == 7)
   {
      // Guard.
      if (tB[7] != 126)
      {
         printf("ESCAPE ERROR 701 %d\n", tB[7]);
         return false;
      }

      // Test the bytes.
      tFound = false;
      if (tB[3] == 49 && tB[4] == 49) { aRecord->mCode = cKey_Function;  aRecord->mChar = 1; tFound = true; }
      if (tB[3] == 49 && tB[4] == 50) { aRecord->mCode = cKey_Function;  aRecord->mChar = 2; tFound = true; }
      if (tB[3] == 49 && tB[4] == 51) { aRecord->mCode = cKey_Function;  aRecord->mChar = 3; tFound = true; }
      if (tB[3] == 49 && tB[4] == 52) { aRecord->mCode = cKey_Function;  aRecord->mChar = 4; tFound = true; }
      if (tB[3] == 49 && tB[4] == 53) { aRecord->mCode = cKey_Function;  aRecord->mChar = 5; tFound = true; }
      if (tB[3] == 49 && tB[4] == 55) { aRecord->mCode = cKey_Function;  aRecord->mChar = 6; tFound = true; }
      if (tB[3] == 49 && tB[4] == 56) { aRecord->mCode = cKey_Function;  aRecord->mChar = 7; tFound = true; }
      if (tB[3] == 49 && tB[4] == 57) { aRecord->mCode = cKey_Function;  aRecord->mChar = 8; tFound = true; }
      if (tB[3] == 50 && tB[4] == 48) { aRecord->mCode = cKey_Function;  aRecord->mChar = 9; tFound = true; }
      if (tB[3] == 50 && tB[4] == 49) { aRecord->mCode = cKey_Function;  aRecord->mChar = 10; tFound = true; }
      if (tB[3] == 50 && tB[4] == 52) { aRecord->mCode = cKey_Function;  aRecord->mChar = 11; tFound = true; }
      if (tB[3] == 50 && tB[4] == 53) { aRecord->mCode = cKey_Function;  aRecord->mChar = 12; tFound = true; }

      // Guard.
      if (!tFound)
      {
         printf("ESCAPE ERROR 702 %d %d\n", tB[3], tB[4]);
         return false;
      }

      // Test more bytes.
      tFound = false;
      if (tB[5] == 59 && tB[6] == 50) { aRecord->mIsShift = true; tFound = true; }
      if (tB[5] == 59 && tB[6] == 51) { aRecord->mIsAlt = true; tFound = true; }
      if (tB[5] == 59 && tB[6] == 52) { aRecord->mIsControl = true; tFound = true; }
      if (tB[5] == 59 && tB[6] == 53) { aRecord->mIsAltShift = true; tFound = true; }
      if (tB[5] == 59 && tB[6] == 54) { aRecord->mIsControlShift = true; tFound = true; }

      // Guard.
      if (!tFound)
      {
         printf("ESCAPE ERROR 703 %d %d\n", tB[5], tB[6]);
         return false;
      }

      // Done.
      return true;
   }

   // Guard.
   printf("ESCAPE ERROR 801 %d\n",tCount);
   return false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace


#if 0
escape sequence bytes.

27 91 65                     up    arrow
27 91 66                     down  arrow
27 91 67                     right arrow
27 91 68                     left  arrow

27 91 49 126                 home
27 91 50 126                 insert
27 91 51 126                 delete
27 91 52 126                 end
27 91 53 126                 page up
27 91 54 126                 page down

27 91 49 49       126   F1
27 91 49 49 59 50 126   F1   shift
27 91 49 49 59 51 126   F1   alt
27 91 49 49 59 52 126   F1   cntl
27 91 49 49 59 53 126   F1   alt   shift
27 91 49 49 59 54 126   F1   cntl  shift

27 91 49 49       126   F1
27 91 49 49 59 50 126   F1   shift

27 91 49 50       126   F2
27 91 49 50 59 50 126   F2   shift

27 91 49 51       126   F3
27 91 49 51 59 50 126   F3   shift

27 91 49 52       126   F4
27 91 49 52 59 50 126   F4   shift

27 91 49 53       126   F5
27 91 49 53 59 50 126   F5   shift

27 91 49 55       126   F6
27 91 49 55 59 50 126   F6   shift

27 91 49 56       126   F7
27 91 49 56 59 50 126   F7   shift

27 91 49 57       126   F8
27 91 49 57 59 50 126   F8   shift

27 91 50 48       126   F9
27 91 50 48 59 50 126   F9   shift

27 91 50 49       126   F10
27 91 50 49 59 50 126   F10  shift

27 91 50 51       126   F11
27 91 50 52 59 50 126   F11  shift

27 91 50 52       126   F12
27 91 50 53 59 50 126   F12  shift

#endif
