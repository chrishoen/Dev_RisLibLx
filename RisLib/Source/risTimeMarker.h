#pragma once
/*==============================================================================
Class that encapsulate a time marker that can be used to measure computer
execution time durations.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risStatistics.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Time marker that runs periodic statistics on the time measurements.

class PeriodicTimeMarker
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Timer counter values at start and stop of 
   // measured time interval.
   long long mTimeCountAtStart;
   long long mTimeCountAtStop;

   // Difference between stop and start time counts, in microseconds
   float mTimeDifferenceUS;

   // Scale factor used to calculate the time difference
   float mScaleFactorUS;

   // True if a start has occurred
   bool mStartFlag;
   
   // Statistics that are calculated for measured
   // time interval

   Ris::PeriodicStatistics mStatistics;
   int mChangeCount;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Intrastructure.

   // Constructor.
   PeriodicTimeMarker();
   void initialize(int aWindowSize);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Start/stop the time measurement.
   void doStart();
   void doStop();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Time marker that runs trial statistics on the time measurements.

class TrialTimeMarker
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Timer counter values at start and stop of 
   // measured time interval.

   long long mTimeCountAtStart;
   long long mTimeCountAtStop;

   // Difference between stop and start time counts, in microseconds
   float mTimeDifferenceUS;

   // Scale factor used to calculate the time difference
   float mScaleFactorUS;

   // True if a start has occurred
   bool mStartFlag;
   
   // Statistics that are calculated for measured
   // time interval
   Ris::TrialStatistics mStatistics;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Intrastructure.

   // Constuctor.
   TrialTimeMarker();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Start/finish trial.
   void startTrial(double aXLimit=0.0);
   void finishTrial();

   // Start/stop the time measurement.
   void doStart();
   void doStop();

};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

