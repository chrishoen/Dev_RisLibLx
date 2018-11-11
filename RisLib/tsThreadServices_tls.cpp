//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "tsThreadLocal.h"
#include "tsThreadServices.h"

namespace TS
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread local storage.

// Thread local storage.
static thread_local ThreadLocal* gThreadLocal = 0;

//***************************************************************************
//***************************************************************************
//***************************************************************************
// Access to thread local storage.

// Set the pointer to the thread local storage. This should be called
// at the begining of the thread run function.
void setThreadLocal(ThreadLocal* aThreadLocal)
{
   gThreadLocal = aThreadLocal;
}

// Return a pointer to the thread local storage.
ThreadLocal* tls()
{
   // Guard.
   if (gThreadLocal == 0)
   {
      printf("gThreadLocal not initialized, exiting program\n");
      exit(1);
      return 0;
   }
   // Return the pointer.
   return gThreadLocal;
}

// Return true if the thread local storage is inititialized.
bool isEnabled()
{
   return gThreadLocal != 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
} //namespace

