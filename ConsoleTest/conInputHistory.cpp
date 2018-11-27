/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <ctype.h>

#include "conInputHistory.h"

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
}

void InputHistory::show()
{
   Prn::print(Prn::View21, "History     %3d $ %3d %3d %3d $",
      mSize,
      mNewest,
      mNextNewest,
      mOldest);
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

   // Set the newest index.
   mNewest = mNextNewest;

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
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
