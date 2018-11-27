/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <ctype.h>

#define  _CONSTRINGREADER_CPP_
#include "conInputReader.h"

namespace Con
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

InputReader::InputReader()
{
   resetVariables();
}

void InputReader::resetVariables()
{
   mCursor = 0;
   mInputLength = 0;
   mInputString[0] = 0;
   mOutputString[0] = 0;
}

void InputReader::initialize()
{
   gKeyReader.initialize();
}

void InputReader::finalize()
{
   gKeyReader.finalize();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Run test loop.

void InputReader::doTestLoop1()
{
   Prn::print(Prn::View11, "doTestLoop1****************************");

   resetVariables();

   while (true)
   {
      // Read the next keyboard input.
      gKeyReader.readKey(&mKeyIn);

      // Update the current input string length.
      mInputLength = (int)strlen(mInputString);

      // Test the input key.
      if (mKeyIn.mIsEndOfRead)
      {
         Prn::print(Prn::View11, "end of read");
         gKeyReader.writeString("\r\n");
         break;
      }

      // Test the input key.
      switch (mKeyIn.mCode)
      {
      case cKey_Ignore:     onKey_Ignore(); break;
      case cKey_Enter:      onKey_Enter(); break;
      case cKey_BackSpace:  onKey_BackSpace(); break;
      case cKey_Delete:     onKey_Delete(); break;
      case cKey_LeftArrow:  onKey_LeftArrow(); break;
      case cKey_RightArrow: onKey_RightArrow(); break;
      case cKey_UpArrow:    onKey_UpArrow(); break;
      case cKey_DownArrow:  onKey_DownArrow(); break;
      case cKey_Home:       onKey_Home(); break;
      case cKey_End:        onKey_End(); break;
      case cKey_Printable:  onKey_Printable(); break;
      case cKey_Special:    onKey_Special(); break;
      }

      // Update the updated input string length.
      mInputLength = (int)strlen(mInputString);

      // Echo the input string.
      echoInput();

      Prn::print(Prn::View11, "mInput %3d $ %4d $  %3d %s",
         mCursor, 
         mKeyIn.mCode, 
         mInputLength,
         mInputString);
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

void InputReader::onKey_Ignore()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Set the cursor fro the beginning.

void InputReader::onKey_Enter()
{
   // Start a newline.
   gKeyReader.writeString("\r\n");

   // Empty the input string.
   mInputString[0] = 0;

   // Set the cursor to the beginning of the empty input string.
   mCursor = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Move the cursor left by one (backspace) and delete the character there. 

void InputReader::onKey_BackSpace()
{ 
   // Move the cursor left by one and delete the character there.
   if (mCursor == 0) return;
   mCursor--;
   onKey_Delete();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Shift the input string left by one, starting at the cursor. This overwrites
// the character at the cursor with the one after it and reduces the length of
// the input string by one.

void InputReader::onKey_Delete()
{ 
   // If the cursor is at the end of the input string then exit.
   if (mCursor == mInputLength) return;

   // Shift the input string from the cursor to the end left by one.
   // This over writes the character at the cursor.
   for (int i = mCursor; i < mInputLength; i++)
   {
      mInputString[i] = mInputString[i + 1];
   }

   // Update the end of the input string.
   mInputString[mInputLength - 1] = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Write one backspace. This moves the cursor left by one.

void InputReader::onKey_LeftArrow()
{
   // Move the cursor left by one.
   if (mCursor == 0) return;
   mCursor--;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Write the input string character at the cursor. This moves the cursor to
// the right by one.

void InputReader::onKey_RightArrow()
{ 
   // Move the cursor right by one.
   if (mCursor == mInputLength) return;
   mCursor++;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void InputReader::onKey_UpArrow()
{ 
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void InputReader::onKey_DownArrow()
{ 
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Write a line feed to move the cursor to the beginning and set the cursor
// to zero.

void InputReader::onKey_Home()
{ 
   // Set the cursor to the beginning of the input string.
   mCursor = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Move the cursor to the end by writing the input string from the cursor
// to the end.

void InputReader::onKey_End()
{
   // Set the cursor to the end of the input string.
   mCursor = mInputLength;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Move the cursor to the end by writing the input string from the cursor
// to the end.

void InputReader::onKey_Special()
{
   gKeyReader.writeString("\r\e[2K");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Insert the input key into the input string at the cursor. Write the
// new string, including the new character, from the cursor to the end.
// Go back from the end to the new cursor.

void InputReader::onKey_Printable()
{ 
   // Shift right by one all keys at and to the right of the cursor.
   for (int i = mInputLength; i > mCursor; i--)
   {
      mInputString[i] = mInputString[i - 1];
   }
   mInputString[mInputLength + 1] = 0;

   // Set the input key at the cursor.
   mInputString[mCursor] = mKeyIn.mChar;

   // Update the cursor.
   mCursor++;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Write the input string to the console output and position the cursor.
// This takes mInputString and mCursor and echos to the console output
// appropriately.

void InputReader::echoInput()
{
   // Hide the cursor.
   gKeyReader.writeString("\e[?25l");

   // Goto the beginning of the line and delete the entire line.
   gKeyReader.writeString("\r\e[2K");

   // Output the input string from the beginning of the line.
   gKeyReader.writeString(mInputString);

   // If the cursor is at zero.
   if (mCursor == 0)
   {
      // Move the cursor to the beginning of the line.
      sprintf(mOutputString, "\r");
   }
   else
   {
      // Move the cursor to the beginning of the line and then to the
      // right by the cursor number.
      sprintf(mOutputString, "\r\e[%dC", mCursor);
   }

   // Write the cursor output string.
   gKeyReader.writeString(mOutputString);

   // Show the cursor.
   gKeyReader.writeString("\e[?25h");

   // Sleep.
   Ris::portableSleep(25);

}


//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
