/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <ctype.h>

#define  _CONSTRINGREADER_CPP_
#include "conStringReader.h"

namespace Con
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

StringReader::StringReader()
{
   resetVariables();
}

void StringReader::resetVariables()
{
   mCursor = 0;
   mLastCursor = 0;
   mDeltaCursor = 0;
   mInputLength = 0;
   mLastInputLength = 0;
   mDeltaInputLength = 0;
   mInputString[0] = 0;
   mOutputString[0] = 0;
}

void StringReader::initialize()
{
   gKeyReader.initialize();
}

void StringReader::finalize()
{
   gKeyReader.finalize();
}

void StringReader::delay()
{
   Ris::portableSleep(100);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Run test loop.

void StringReader::doTestLoop1()
{
   Prn::print(Prn::View11, "doTestLoop1****************************");

   resetVariables();

   while (true)
   {
      mLastInputLength = mInputLength;
      mLastCursor = mCursor;

      gKeyReader.readKey(&mKeyIn);

      if (mKeyIn.mIsEndOfRead)
      {
         Prn::print(Prn::View11, "end of read");
         gKeyReader.writeNewLine();
         break;
      }
#if 0
      Prn::print(Prn::View11, "mKeyIn %4d %c",
         mKeyIn.mCode,
         mKeyIn.mChar);

      continue;
#endif
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

      mDeltaCursor = mCursor - mLastCursor;
      mInputLength = (int)strlen(mInputString);
      mDeltaInputLength = mInputLength - mLastInputLength;

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

void StringReader::doTouchCursor()
{
   return;
   if (mInputLength < 2) return;
   if (mCursor == 0)
   {
      gKeyReader.writeOne(mInputString[0]);
      gKeyReader.writeLeftOne();
   }
   else if (mCursor == mInputLength)
   {
      gKeyReader.writeLeftOne();
      gKeyReader.writeOne(mInputString[mCursor - 1]);
   }
   else
   {
      gKeyReader.writeLeftOne();
      gKeyReader.writeOne(mInputString[mCursor - 1]);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void StringReader::onKey_Ignore()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Set the cursor fro the beginning.

void StringReader::onKey_Enter()
{
   mCursor = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Move the cursor left by one (backspace) and delete the character there. 

void StringReader::onKey_BackSpace()
{ 
   if (mCursor == 0) return;
   gKeyReader.writeLeftOne();
   mCursor--;
   onKey_Delete();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Shift the input string left by one, starting at the cursor. This overwrites
// the character at the cursor with the one after it and reduces the length of
// the input string by one.

void StringReader::onKey_Delete()
{ 
   // If at the end then exit.
   if (mCursor == mInputLength) return;

   // Shift the input string from the cursor to the end left by one
   // and write it out. This over writes the character at the cursor.
   for (int i = mCursor; i < mInputLength; i++)
   {
      mInputString[i] = mInputString[i + 1];
      gKeyReader.writeOne(mInputString[i]);
   }
   mInputString[mInputLength - 1] = 0;
   mInputLength--;
   // Erase the last character.
   gKeyReader.writeOne(' ');

   // Go back from the end to the cursor.
   int tCount = mInputLength - mCursor + 2;
   for (int i = 0; i < tCount; i++)
   {
      gKeyReader.writeLeftOne();
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Write one backspace. This moves the cursor left by one.

void StringReader::onKey_LeftArrow()
{
   if (mCursor == 0) return;
   gKeyReader.writeLeftOne();
   mCursor--;
   return;

   if (mCursor == 0) return;
   mCursor--;
   echoInput();
   return;

}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Write the input string character at the cursor. This moves the cursor to
// the right by one.

void StringReader::onKey_RightArrow()
{ 
   if (mCursor < mInputLength)
   {
      gKeyReader.writeOne(mInputString[mCursor]);
      mCursor++;
   }
   return;

   mCursor++;
   echoInput();
   return;

}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void StringReader::onKey_UpArrow()
{ 
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void StringReader::onKey_DownArrow()
{ 
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Write a line feed to move the cursor to the beginning and set the cursor
// to zero.

void StringReader::onKey_Home()
{ 
   if (mCursor == 0) return;
   gKeyReader.writeOne(13);
   mCursor = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Move the cursor to the end by writing the input string from the cursor
// to the end.

void StringReader::onKey_End()
{
   while (mCursor < mInputLength)
   {
      gKeyReader.writeOne(mInputString[mCursor]);
      mCursor++;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Move the cursor to the end by writing the input string from the cursor
// to the end.

void StringReader::onKey_Special()
{
   gKeyReader.writeOne(77);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Insert the input key into the input string at the cursor. Write the
// new string, including the new character, from the cursor to the end.
// Go back from the end to the new cursor.

void StringReader::onKey_Printable()
{ 
   // Shift right by one all keys at and to the right of the cursor.
   mInputLength = (int)strlen(mInputString);
   for (int i = mInputLength; i > mCursor; i--)
   {
      mInputString[i] = mInputString[i - 1];
   }
   mInputString[mInputLength + 1] = 0;
   mInputLength++;

   // Set the input key at the cursor.
   mInputString[mCursor] = mKeyIn.mChar;

   // Write the new string from the cursor to the end.
   for (int i = mCursor; i < mInputLength; i++)
   {
      gKeyReader.writeOne(mInputString[i]);
   }

   // Set the new cursor and go back to it.
   mCursor++;
   for (int i = mCursor; i < mInputLength; i++)
   {
      gKeyReader.writeLeftOne();
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Write the output string to the console output.

void StringReader::echoInput()
{
   // Get the input string length.
   mInputLength = (int)strlen(mInputString);

   // If the input string length did not change.
   if (mDeltaInputLength == 0)
   {
      // Write the input string from the beginning of the line to the
      // end of the string.
      strcpy(mOutputString, "\r");
      strncat(mOutputString, mInputString, cMaxStringSize);
      gKeyReader.writeString(mOutputString);

      // Write the input string from the beginning of the line to the
      // cursor.
      if (mCursor < mInputLength)
      {
         // Write the input string from the beginning of the line to the
         // cursor.
         strcpy(mOutputString, "\r");
         strncat(mOutputString, mInputString, cMaxStringSize);
         mOutputString[mCursor] = 0;
         gKeyReader.writeString(mOutputString);
      }

   }

}


//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
