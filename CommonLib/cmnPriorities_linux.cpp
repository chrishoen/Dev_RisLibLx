/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#define  _CMNPRIORITIES_CPP_
#include "cmnPriorities.h"

namespace Cmn
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

Priorities::Priorities()
{
   mTsPrint           = Ris::Threads::Priority(-1, 20);
   mMasterLong        = Ris::Threads::Priority(-1, 50);
   mMasterShort       = Ris::Threads::Priority(-1, 80);
   mSlave             = Ris::Threads::Priority(-1, 50);
   mTimer             = Ris::Threads::Priority(-1, 80);
   mTest              = Ris::Threads::Priority(-1, 50);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
