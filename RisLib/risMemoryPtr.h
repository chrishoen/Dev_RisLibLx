#pragma once

/*==============================================================================
Memory pointer class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Memory Pointer class.

class MemoryPtr
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Memory pointer.
   void* mMemoryPtr;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Intrastructure.

   // Constructors.
   MemoryPtr();
   MemoryPtr(void* aMemoryPtr);
   void set(void* aMemoryPtr);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Get memory pointer value.
   void* vfetch();
   char* cfetch();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Null pointer value.
   void setNull();
   bool isNull();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Add a number of bytes to the memory pointer.
   // Return the original memory pointer as a void*.
   // This changes the memory pointer.
   void* vfetch_add(int aNumBytes);

   // Add a number of bytes to the memory pointer.
   // Return the original memory pointer as a char*.
   // This changes the memory pointer.
   char* cfetch_add(int aNumBytes);

   // Subtract a number of bytes from the memory pointer.
   // Return the original memory pointer as a void*.
   // This changes the memory pointer.
   void* vfetch_sub(int aNumBytes);

   // Subtract a number of bytes from the memory pointer.
   // Return the original memory pointer as a char*.
   // This changes the memory pointer.
   char* cfetch_sub(int aNumBytes);

   // Return the memory pointer plus a number of bytes as a void*.
   // This does not change the memory pointer.
   void* vplus(int aNumBytes);

   // Return the memory pointer plus a number of bytes as a char*.
   // This does not change the memory pointer.
   char* cplus(int aNumBytes);

   // Return the memory pointer minus a number of bytes as a void*.
   // This does not change the memory pointer.
   void* vminus(int aNumBytes);

   // Return the memory pointer minus a number of bytes as a char*.
   // This does not change the memory pointer.
   char* cminus(int aNumBytes);

};

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructors.

inline MemoryPtr::MemoryPtr()
{
   mMemoryPtr = 0;
}

inline MemoryPtr::MemoryPtr(void* aMemoryPtr)
{
   mMemoryPtr  = aMemoryPtr;
}

inline void MemoryPtr::set(void* aMemoryPtr)
{
   mMemoryPtr  = aMemoryPtr;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Get memory pointer value.

inline void* MemoryPtr::vfetch()
{
   return mMemoryPtr;
}

inline char* MemoryPtr::cfetch()
{
   return (char*)mMemoryPtr;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Set null.

inline void MemoryPtr::setNull()
{
   mMemoryPtr = 0;
}

inline bool MemoryPtr::isNull()
{
   return mMemoryPtr==0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Add a number of bytes to the memory pointer.
// Return the original memory pointer as a void*.
// This changes the memory pointer.

inline void* MemoryPtr::vfetch_add(int aNumBytes)
{
   void* tVOriginalPtr = mMemoryPtr;
   char* tCPtr = ((char*)mMemoryPtr) + aNumBytes;
   mMemoryPtr  = (void*)tCPtr;
   return tVOriginalPtr;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Add a number of bytes to the memory pointer.
// Return the original memory pointer as a char*.
// This changes the memory pointer.

inline char* MemoryPtr::cfetch_add(int aNumBytes)
{
   char* tCOriginalPtr = (char*)mMemoryPtr;
   char* tCPtr = ((char*)mMemoryPtr) + aNumBytes;
   mMemoryPtr  = (void*)tCPtr;
   return tCOriginalPtr;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Subtract a number of bytes from the memory pointer.
// Return the original memory pointer as a void*.
// This changes the memory pointer.

inline void* MemoryPtr::vfetch_sub(int aNumBytes)
{
   void* tVOriginalPtr = mMemoryPtr;
   char* tCPtr = ((char*)mMemoryPtr) - aNumBytes;
   mMemoryPtr  = (void*)tCPtr;
   return tVOriginalPtr;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Subtract a number of bytes from the memory pointer.
// Return the original memory pointer as a char*.
// This changes the memory pointer.

inline char* MemoryPtr::cfetch_sub(int aNumBytes)
{
   char* tCOriginalPtr = (char*)mMemoryPtr;
   char* tCPtr = ((char*)mMemoryPtr) - aNumBytes;
   mMemoryPtr  = (void*)tCPtr;
   return tCOriginalPtr;
}

//***************************************************************************
//***************************************************************************
//***************************************************************************
// Return the memory pointer plus a number of bytes as a void*.
// This does not change the memory pointer.

inline void* MemoryPtr::vplus(int aNumBytes)
{
   char*  tCPtr = ((char*)mMemoryPtr) + aNumBytes;
   void*  tVPtr = (void*)tCPtr;
   return tVPtr;
}

//***************************************************************************
//***************************************************************************
//***************************************************************************
// Return the memory pointer plus a number of bytes as a char*.
// This does not change the memory pointer.

inline char* MemoryPtr::cplus(int aNumBytes)
{
   char*  tCPtr = ((char*)mMemoryPtr) + aNumBytes;
   return tCPtr;
}

//***************************************************************************
//***************************************************************************
//***************************************************************************
// Return the memory pointer minus a number of bytes as a void*.
// This does not change the memory pointer.

inline void* MemoryPtr::vminus(int aNumBytes)
{
   char*  tCPtr = ((char*)mMemoryPtr) - aNumBytes;
   void*  tVPtr = (void*)tCPtr;
   return tVPtr;
}

//***************************************************************************
//***************************************************************************
//***************************************************************************
// Return the memory pointer minus a number of bytes as a char*.
// This does not change the memory pointer.

inline char* MemoryPtr::cminus(int aNumBytes)
{
   char*  tCPtr = ((char*)mMemoryPtr) - aNumBytes;
   return tCPtr;
}

//***************************************************************************
//***************************************************************************
//***************************************************************************
}//namespace
