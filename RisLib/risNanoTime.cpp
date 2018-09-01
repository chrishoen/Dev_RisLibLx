/*==============================================================================
==============================================================================*/
#include "stdafx.h"

#include <time.h>

#include "risNanoTime.h"

namespace Ris
{
namespace NanoTime
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Time units

enum Units
{
   Units_Sec,  // Seconds
   Units_Ms,   // Milliseconds
   Units_Us,   // Microseconds
   Units_Ns    // Nanoseconds
};

long long rNsPerUnit[4] = {1000*1000*1000,1000*1000,1000,1};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Alias for timespec

struct my_timespec {
    time_t tv_sec;      /* Seconds */
    long   tv_nsec;     /* Nanoseconds [0 .. 999999999] */
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Unit conversions

// Nanoseconds from seconds
long long  getNsFromSec  (double aTimeSec)
{
   return (long long)(aTimeSec*rNsPerUnit[Units_Sec]);
}

// Nanoseconds from milliseconds
long long  getNsFromMs   (int    aTimeMs)
{
   return (long long)(aTimeMs*rNsPerUnit[Units_Ms]);
}

// Nanoseconds from microseconds
long long  getNsFromUs   (int    aTimeUs)
{
   return (long long)(aTimeUs*rNsPerUnit[Units_Us]);
}

// Seconds from nanoseconds
double getSecFromNs  (long long  aTimeNs)
{
   return (double)(aTimeNs/rNsPerUnit[Units_Sec]);
}

// Milliseconds from nanoseconds
int    getMsFromNs   (long long  aTimeNs)
{
   return (int)(aTimeNs/rNsPerUnit[Units_Ms]);
}

// Microseconds from nanoseconds
int    getUsFromNs   (long long  aTimeNs)
{
   return (int)(aTimeNs/rNsPerUnit[Units_Us]);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// timespec conversions

void getNsFromTimespec (long long&  aTimeNs, void* aTimespec)
{
   my_timespec tTimespec;
   memcpy((void*)&tTimespec,aTimespec,sizeof(my_timespec));
   aTimeNs = (long long)(tTimespec.tv_sec)*rNsPerUnit[Units_Sec] + (long long)tTimespec.tv_nsec;
}

void getTimespecFromNs (void* aTimespec, long long aTimeNs)
{
   my_timespec* tTimespec = (my_timespec*)aTimespec;
   tTimespec->tv_sec   = (int)(aTimeNs / rNsPerUnit[Units_Sec]);
   tTimespec->tv_nsec  = (int)(aTimeNs % rNsPerUnit[Units_Sec]);
   if (tTimespec->tv_nsec > 999999999) tTimespec->tv_nsec = 999999999;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// tm conversions

void getNsFromTm (long long&  aTimeNs, struct tm* aTm)
{
   my_timespec tTimespec;
   tTimespec.tv_sec  = mktime(aTm);
   tTimespec.tv_nsec = 0;

   getNsFromTimespec(aTimeNs,&tTimespec);
}

void getTmFromNs (struct tm* aTm,  long long aTimeNs)
{
   my_timespec tTimespec;
   getTimespecFromNs(&tTimespec,aTimeNs);

   tm* tTm = localtime(&tTimespec.tv_sec);
   memcpy((void*)aTm,tTm,sizeof(tm));
}

//******************************************************************************
#if 0

char* NanoTime::asString1(char* aString)
{
   int tYear;
   int tDayInYear;
   int tHours;
   int tMinutes;
   TimeDuration tSeconds;

   convertToIrig(tYear,tDayInYear,tHours,tMinutes,tSeconds);

   int tSecond=0;
   int tHalfNs=0;
   tSeconds.convertToSecondsAndHalfNanoseconds(tSecond,tHalfNs);

   sprintf(aString,"%04d:%03d:%02d:%02d:%02d",tYear,tDayInYear,tHours,tMinutes,tSecond);
   return aString;
}

//******************************************************************************

char* NanoTime::asString2(char* aString)
{
   int tYear;
   int tDayInYear;
   int tHours;
   int tMinutes;
   TimeDuration tSeconds;

   convertToIrig(tYear,tDayInYear,tHours,tMinutes,tSeconds);

   sprintf(aString,"%04d\t%03d\t%02d\t%02d\t%012.9lf",tYear,tDayInYear,tHours,tMinutes,tSeconds.inSeconds());
   return aString;
}

//******************************************************************************
char* NanoTime::asString3(char* aString)
{
   int tYear;
   int tDayInYear;
   int tHours;
   int tMinutes;
   TimeDuration tSeconds;

   convertToIrig(tYear,tDayInYear,tHours,tMinutes,tSeconds);

   sprintf(aString,"%04d:%03d:%02d:%02d:%012.9lf",tYear,tDayInYear,tHours,tMinutes,tSeconds.inSeconds());
   return aString;
}
#endif
}//namespace
}//namespace

