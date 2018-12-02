/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "my_functions.h"
#include "prnPrint.h"

#include <ctype.h>

#include "conInputHistory_linux.h"

namespace Con
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

InputHistory::InputHistory()
{
   resetVariables();
}

void InputHistory::resetVariables()
{
   for (int i = 0; i < cArraySize; i++)
   {
      mStringArray[i][0] = 0;
      mPtr[i] = &mStringArray[i][0];
   }

   mOldest = 0;
   mNewest = 0;
   mNextNewest = 0;
   mSize = 0;
   mUpCount = 0;
}

void InputHistory::show()
{
   return;
   Prn::print(Prn::View21, "History     %3d $ %3d %3d %3d $ %3d",
      mSize,
      mNewest,
      mNextNewest,
      mOldest,
      mUpCount);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Put a string to the array for when enter is entered at 
// the console input.

void InputHistory::putStringForEnter(char* aInputString)
{
   // Guard.
   if (strlen(aInputString) == 0) return;

   // Copy the input string to the array at the next newest index.
   my_strncpy(mPtr[mNextNewest], aInputString, cStringSize);

   // Set the newest index so that it is at the most recent.
   mNewest = mNextNewest;

   // Reset up count.
   mUpCount = 0;

   // Increment the next newest index,
   // wrap around the array.
   mNextNewest = my_index_add(mNextNewest, 1, cArraySize);

   // Increment the size up to the array size.
   if (++mSize > cArraySize) mSize = cArraySize;

   // If the array is full then increment the oldest index,
   // wrap around the array.
   if (mSize == cArraySize)
   {
      mOldest = my_index_add(mOldest, 1, cArraySize);
   }

   // Done.
   show();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Get a string from the array for when an up arrow is entered at
// the console input.

void InputHistory::getStringForUpArrow(char* aOutputString)
{
   // Guard.
   if (mSize == 0) return;
   if (mUpCount == mSize) return;

   // Increment the up count.
   mUpCount++;

   // Calculate the get index, wrap around the array.
   int tGetIndex = my_index_sub(mNewest, mUpCount - 1, cArraySize);

   // Copy the array string at the get index to the output string.
   my_strncpy(aOutputString, mPtr[tGetIndex], cStringSize);

   // Done.
   show();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Get a string from the array for when an up arrow is entered at
// the console input.

void InputHistory::getStringForDownArrow(char* aOutputString)
{
   // Guard.
   if (mSize == 0) return;
   if (mUpCount == 0) return;

   // Decrement the up count.
   mUpCount--;

   // If the up count is zero.
   if (mUpCount == 0)
   {
      // Return an empty string.
      aOutputString[0] = 0;
      return;
   }

   // Calculate the get index, wrap around the array.
   int tGetIndex = my_index_sub(mNewest, mUpCount - 1, cArraySize);

   // Copy the array string at the get index to the output string.
   my_strncpy(aOutputString, mPtr[tGetIndex], cStringSize);

   // Done.
   show();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
