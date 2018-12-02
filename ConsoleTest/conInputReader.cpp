/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <ctype.h>

#define  _CONINPUTREADER_CPP_
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
   mInputHistory.resetVariables();
}

void InputReader::initialize()
{
   resetVariables();
   gKeyReader.mPF = false;
   gKeyReader.initialize();
}

void InputReader::finalize()
{
   gKeyReader.finalize();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Read a string from the console input and copy it to the input
// argument.

void InputReader::doReadString(char* aInputString)
{
   int tFound = 0;

   mCursor = 0;
   mInputLength = 0;
   mInputString[0] = 0;

   while (true)
   {
      // Read the next keyboard input.
      gKeyReader.readKey(&mKeyIn);

      // Update the current input string length.
      mInputLength = (int)strlen(mInputString);

      // Test the input key.
      tFound = 0;
      switch (mKeyIn.mCode)
      {
      case cKey_BackSpace:  onKey_BackSpace();  tFound = 1; break;
      case cKey_Delete:     onKey_Delete();     tFound = 1; break;
      case cKey_LeftArrow:  onKey_LeftArrow();  tFound = 1; break;
      case cKey_RightArrow: onKey_RightArrow(); tFound = 1; break;
      case cKey_UpArrow:    onKey_UpArrow();    tFound = 1; break;
      case cKey_DownArrow:  onKey_DownArrow();  tFound = 1; break;
      case cKey_Home:       onKey_Home();       tFound = 1; break;
      case cKey_End:        onKey_End();        tFound = 1; break;
      case cKey_Insert:     onKey_Insert();     tFound = 1; break;
      case cKey_PageUp:     onKey_PageUp();     tFound = 1; break;
      case cKey_PageDown:   onKey_PageDown();   tFound = 1; break;
      case cKey_Printable:  onKey_Printable();  tFound = 1; break;

      case cKey_Enter:      onKey_Enter();      tFound = 2; break;
      case cKey_Control:    onKey_Control();    tFound = 3; break;
      case cKey_Alt:        onKey_Alt();        tFound = 3; break;
      case cKey_Function:   onKey_Function();   tFound = 3; break;
      case cKey_Escape:     onKey_Escape();     tFound = 3; break;
      }

      // Update the new input string length.
      mInputLength = (int)strlen(mInputString);

      // Guard.
      if (tFound==0)
      {
         printf("InputReader ERROR not found\n");
         return;
      }

      // Echo the input string.
      if (tFound == 1)
      {
         echoInput();
      }

      // Copy the input string to the argument input string
      // and exit the loop.
      if (tFound == 2)
      {
         strcpy(aInputString,mInputString);
         return;
      }

      // Copy the input string to the argument input string
      // and exit the loop.
      if (tFound == 3)
      {
         echoEmpty();
         strcpy(aInputString, mInputString);
         return;
      }
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

void InputReader::onKey_Enter()
{
   // Put the current input string to the input string history.
   mInputHistory.putStringForEnter(mInputString);

   // Start a newline.
   gKeyReader.writeString("\r\n");

   // Set the cursor to the beginning of the empty input string.
   mCursor = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void InputReader::onKey_UpArrow()
{
   // Get the input string from the input string history.
   mInputHistory.getStringForUpArrow(mInputString);

   // Set the cursor to the end of the new input string.
   mCursor = strlen(mInputString);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void InputReader::onKey_DownArrow()
{
   // Get the input string from the input string history.
   mInputHistory.getStringForDownArrow(mInputString);

   // Set the cursor to the end of the new input string.
   mCursor = strlen(mInputString);
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
// Write to the input string and set the cursor to the beginning.

void InputReader::onKey_Insert()
{
   sprintf(mInputString, "insert");
   mCursor = 0;
}

void InputReader::onKey_PageUp()
{
   sprintf(mInputString, "pageup");
   mCursor = 0;
}

void InputReader::onKey_PageDown()
{
   sprintf(mInputString, "pagedown");
   mCursor = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Write to the input string and set the cursor to the beginning.

void InputReader::onKey_Control()
{
   sprintf(mInputString, "cntl_%c", mKeyIn.mChar);
   mCursor = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Write to the input string and set the cursor to the beginning.

void InputReader::onKey_Alt()
{
   sprintf(mInputString, "alt_%c", mKeyIn.mChar);
   mCursor = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Write to the input string and set the cursor to the beginning.

void InputReader::onKey_Function()
{
   if (mKeyIn.mIsShift)            sprintf(mInputString, "shift_fn%d", mKeyIn.mChar);
   else if (mKeyIn.mIsAlt)         sprintf(mInputString, "alt_fn%d", mKeyIn.mChar);
   else if (mKeyIn.mIsControl)     sprintf(mInputString, "cntl_fn%d", mKeyIn.mChar);
   else if (mKeyIn.mIsAltShift)    sprintf(mInputString, "alt_shift_fn%d", mKeyIn.mChar);
   else if(mKeyIn.mIsControlShift) sprintf(mInputString, "cntl_shift_fn%d", mKeyIn.mChar);
   else                            sprintf(mInputString, "fn%d", mKeyIn.mChar);
   mCursor = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Write to the input string and set the cursor to the beginning.

void InputReader::onKey_Escape()
{
   strcpy(mInputString, "escape");
   mCursor = 0;
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
   char tOutputString[200];

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
      sprintf(tOutputString, "\r");
   }
   else
   {
      // Move the cursor to the beginning of the line and then to the
      // right by the cursor number.
      sprintf(tOutputString, "\r\e[%dC", mCursor);
   }

   // Write the cursor output string.
   gKeyReader.writeString(tOutputString);

   // Show the cursor.
   gKeyReader.writeString("\e[?25h");

   // Sleep.
   Ris::portableSleep(25);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Write an empty string to the console output.

void InputReader::echoEmpty()
{
   // Goto the beginning of the line and delete the entire line.
   gKeyReader.writeString("\r\e[2K");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Run test loop.

void InputReader::doTestLoop1()
{
   Prn::print(Prn::View01, "InputReader::doTestLoop1****************************");
   Prn::print(Prn::View11, "InputReader::doTestLoop1****************************");
   Prn::print(Prn::View21, "InputReader::doTestLoop1****************************");

   int tCount = 0;
   char  tString[200];

   while (true)
   {
      doReadString(tString);
      Prn::print(Prn::View11, "%4d $ %4d %s", ++tCount, strlen(tString), tString);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
