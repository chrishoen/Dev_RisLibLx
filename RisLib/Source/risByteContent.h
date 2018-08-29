#pragma once

/*==============================================================================

ByteContent and ByteBuffer classes.

ByteBuffer is a memory buffer that contains data that is binary encoded.

ByteContent is a base class for something that can be copied to/from a
ByteBuffer.

These classes are intended to support the construction of message classes.
The idea here is that each message is encapsulated by a class that inherits
from ByteContent and supplies a copytoFrom member function that allows it 
to be copied to/from a ByteBuffer, which can be then transported via some 
communications medium. 

Copying a message class to a byte buffer entails writing the class member
variables to the buffer, binary encoding them. 

Copying a message class from a byte buffer entails reading the class member
variables from the buffer, binary decoding them.

The binary encoding/decoding of a message class is defined by the copyToFrom.

==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risByteBuffer.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is an abstract base class for classes that can be copied to/from
// byte buffers. Each inheriting class overrides a copyToFrom member function
// that can be called by ByteBuffer put and get functions to copy it.

class ByteContent
{
public:
   // This is overridden by an inheriting class to copy itself to/from a byte 
   // buffer. The buffer has a direction flag. If it is true then the class
   // is copied to the buffer, writing it using a binary encode. If it is false 
   // then it is copied from the buffer, reading it using a binary decode.
   virtual void copyToFrom(Ris::ByteBuffer* aBuffer)=0;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

