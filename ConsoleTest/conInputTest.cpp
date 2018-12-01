/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <ctype.h>

#define  _CONINPUTTEST_CPP_
#include "conInputTest.h"

namespace Con
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

InputTest::InputTest()
{
   resetVariables();
}

void InputTest::resetVariables()
{
   mCursor = 0;
   mInputLength = 0;
   mInputString[0] = 0;
   mOutputString[0] = 0;
   mInputHistory.resetVariables();
}

void InputTest::initialize()
{
   gKeyReader.initialize();
}

void InputTest::finalize()
{
   gKeyReader.finalize();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Run test loop.

void InputTest::doTestLoop2()
{
   printf("doTestLoop2****************************\n");
   while (true)
   {
      int tChar = gKeyReader.readOne();
      if (tChar == ' ')
      {
         printf("\n****************************\n");
      }
      else
      {
         printf(" %3d\n", tChar);
      }

      if (tChar == 'z')
      {
         printf("\nexit\n");
         break;
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Run test loop.

void InputTest::doTestLoop1()
{
   bool tFound = false;

   Prn::print(Prn::View11, "doTestLoop1****************************");
   Prn::print(Prn::View21, "doTestLoop1****************************");

   resetVariables();

   while (true)
   {
      // Read the next keyboard input.
      gKeyReader.readKey(&mKeyIn);

      // Update the current input string length.
      mInputLength = (int)strlen(mInputString);

      // Test the input key.
      tFound = false;
      switch (mKeyIn.mCode)
      {
      case cKey_Enter:      onKey_Enter();      tFound = true; break;
      case cKey_BackSpace:  onKey_BackSpace();  tFound = true; break;
      case cKey_Delete:     onKey_Delete();     tFound = true; break;
      case cKey_LeftArrow:  onKey_LeftArrow();  tFound = true; break;
      case cKey_RightArrow: onKey_RightArrow(); tFound = true; break;
      case cKey_UpArrow:    onKey_UpArrow();    tFound = true; break;
      case cKey_DownArrow:  onKey_DownArrow();  tFound = true; break;
      case cKey_Home:       onKey_Home();       tFound = true; break;
      case cKey_End:        onKey_End();        tFound = true; break;
      case cKey_Insert:     onKey_Insert();     tFound = true; break;
      case cKey_PageUp:     onKey_PageUp();     tFound = true; break;
      case cKey_PageDown:   onKey_PageDown();   tFound = true; break;
      case cKey_Printable:  onKey_Printable();  tFound = true; break;
      case cKey_Control:    onKey_Control();    tFound = true; break;
      case cKey_Alt:        onKey_Alt();        tFound = true; break;
      case cKey_Function:   onKey_Function();   tFound = true; break;
      case cKey_Escape:     onKey_Escape();     tFound = true; break;
      }

      // Guard.
      if (!tFound)
      {
         printf("InputTest ERROR not found\n");
         break;
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

void InputTest::onKey_Enter()
{
   // Put the current input string to the input string history.
   mInputHistory.putStringForEnter(mInputString);

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

void InputTest::onKey_UpArrow()
{
   // Get the input string from the input string history.
   mInputHistory.getStringForUpArrow(mInputString);

   // Set the cursor to the end of the new input string.
   mCursor = strlen(mInputString);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void InputTest::onKey_DownArrow()
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

void InputTest::onKey_BackSpace()
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

void InputTest::onKey_Delete()
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

void InputTest::onKey_LeftArrow()
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

void InputTest::onKey_RightArrow()
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

void InputTest::onKey_Home()
{ 
   // Set the cursor to the beginning of the input string.
   mCursor = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Move the cursor to the end by writing the input string from the cursor
// to the end.

void InputTest::onKey_End()
{
   // Set the cursor to the end of the input string.
   mCursor = mInputLength;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Write to the input string and set the cursor to the beginning.

void InputTest::onKey_Insert()
{
   sprintf(mInputString, "insert");
   mCursor = 0;
}

void InputTest::onKey_PageUp()
{
   sprintf(mInputString, "pageup");
   mCursor = 0;
}

void InputTest::onKey_PageDown()
{
   sprintf(mInputString, "pagedown");
   mCursor = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Write to the input string and set the cursor to the beginning.

void InputTest::onKey_Control()
{
   sprintf(mInputString, "cntl_%c", mKeyIn.mChar);
   mCursor = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Write to the input string and set the cursor to the beginning.

void InputTest::onKey_Alt()
{
   sprintf(mInputString, "alt_%c", mKeyIn.mChar);
   mCursor = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Write to the input string and set the cursor to the beginning.

void InputTest::onKey_Function()
{
   sprintf(mInputString, "function%d", mKeyIn.mChar);

   if (mKeyIn.mIsShift)            sprintf(mInputString, "shift_function%d", mKeyIn.mChar);
   else if (mKeyIn.mIsAlt)         sprintf(mInputString, "alt_function%d", mKeyIn.mChar);
   else if (mKeyIn.mIsControl)     sprintf(mInputString, "cntl_function%d", mKeyIn.mChar);
   else if (mKeyIn.mIsAltShift)    sprintf(mInputString, "alt_shift_function%d", mKeyIn.mChar);
   else if(mKeyIn.mIsControlShift) sprintf(mInputString, "cntl_shift_function%d", mKeyIn.mChar);
   else                            sprintf(mInputString, "function%d", mKeyIn.mChar);
   mCursor = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Write to the input string and set the cursor to the beginning.

void InputTest::onKey_Escape()
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

void InputTest::onKey_Printable()
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

void InputTest::echoInput()
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
