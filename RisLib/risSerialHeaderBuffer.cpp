/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "my_functions.h"

#include "risSerialHeaderBuffer.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

SerialHeaderBuffer::SerialHeaderBuffer()
{
   reset();
}

SerialHeaderBuffer::SerialHeaderBuffer(int aSize)
{
   mArray = new char[aSize];
   mSize  = aSize;
   mCount = 0;
   mIndex = mSize-1;
}

SerialHeaderBuffer::~SerialHeaderBuffer()
{
   finalize();
}

void SerialHeaderBuffer::reset()
{
   mArray = 0;
   mSize  = 0;
   mCount = 0;
   mIndex = 0;
}

void SerialHeaderBuffer::initialize(int aSize)
{
   finalize();
   reset();
   mArray = new char[aSize];
   mSize  = aSize;
   mCount = 0;
   mIndex = mSize-1;
}

void SerialHeaderBuffer::reinitialize()
{
   mCount = 0;
   mIndex = mSize-1;
}

void SerialHeaderBuffer::finalize()
{
   if (mArray)
   {
      delete mArray;
      mArray = 0;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

void SerialHeaderBuffer::shiftUp(char aX)
{
   // Adjust the index down.
   mIndex = my_index_add(mIndex, 1, mSize);

   // Add new value to array.
   mX=aX;
   mArray[mIndex]=aX;

   // Calculate number of elements.
   if (++mCount >= mSize)
   {
      mValid=true;
      mCount = mSize;
   }
   else
   {
      mValid=false;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void SerialHeaderBuffer::setBottom(char aX)
{
   // Set array left end value
   mArray[mIndex]=aX;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

char SerialHeaderBuffer::get(int aOffset)
{
   // Guard
   if (!mValid) return 0;

   // Get index
   int tTopIndex = my_index_add(mIndex, 1, mSize);
   int tGetIndex = my_index_add(tTopIndex, aOffset, mSize);

   // Return array value at index
   return mArray[tGetIndex];
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

char SerialHeaderBuffer::getBottom()
{
   // Guard
   if (!mValid) return 0;

   // Return array value at index
   return mArray[mIndex];
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

char SerialHeaderBuffer::getTop()
{
   // Guard
   if (!mValid) return 0;

   // Get index of right end
   int tGetIndex = my_index_sub(mIndex, (mSize - 1), mSize);

   // Return array value at index
   return mArray[tGetIndex];
}


//******************************************************************************
//******************************************************************************
//******************************************************************************

void SerialHeaderBuffer::show()
{
   printf("SerialHeaderBuffer:                   %3d %3d %1d $$",mIndex,mCount,mValid);
   for (int i=0;i<mSize;i++)
   {
      printf("%3d ", (int)get(i));
   }
   printf("\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Copy the entire header buffer to a byte buffer.

void SerialHeaderBuffer::copyTo(Ris::ByteBuffer* aBuffer)
{
   // Copy header buffer to the byte buffer.
   for (int i=0;i<mSize;i++)
   {
      char tX = get(i);
      aBuffer->copy(&tX);
   }

}
//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

