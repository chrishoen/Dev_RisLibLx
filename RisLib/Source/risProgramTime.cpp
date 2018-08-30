/*==============================================================================
==============================================================================*/

#include "stdafx.h"

#include "risPortableCalls.h"
#include "risProgramTime.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Regionals

// High performance time count at program start
static long long int mProgramStartTimeCount = Ris::portableGetHiResCounter();

// Scale factor used to calculate the current program time
static double mScaleFactorSec = (double)((1.0)/Ris::portableGetHiResFrequency());

   
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Read current time and subtract program start time.
// This returns the current program time in seconds.

double getCurrentProgramTime()
{
   long long int mCurrentTimeCount = Ris::portableGetHiResCounter();
   return (mCurrentTimeCount - mProgramStartTimeCount)*mScaleFactorSec;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constuctor.

ProgramTimeMarker::ProgramTimeMarker()
{
   mElapsed = 0.0;
   mStart = 0.0;
   mStop = 0.0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Start/stop the time measurement.

void ProgramTimeMarker::doStart()
{
   mStart = getCurrentProgramTime();
}

void ProgramTimeMarker::doStop()
{
   mStop = getCurrentProgramTime();
   mElapsed = mStop - mStart;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

