#pragma once

/*==============================================================================
Thread Services.
These provide printing, logging, and error handling services that are
based on having the controlling parameters located in thread local storage.

A global shared object that contains program settings.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdio.h>
#include "tsThreadLocal.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace TS
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constants.

   static const int cMaxStringSize = 400;

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This class provides global program settings storage for the thread
// system printing and logging facility.

class Share
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Thread local storage. This is created in the constructor. The pointer
   // is copied to the thread local storage variable at the beginning of
   // the thread run function.
   TS::ThreadLocal* mMainThreadLocal;

   // Program log file.
   char mProgramName[cMaxStringSize];

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   Share();
   ~Share();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global singular instance.

#ifdef _TSSHARE_CPP_
          Share gShare;
#else
   extern Share gShare;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

