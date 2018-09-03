#include "stdafx.h"


#include "CmdLineExec.h"

#include "risThreadsThreads.h"
#include "risTimeMarker.h"

#include "someTimerThread.h"
using namespace Some;

//******************************************************************************
//******************************************************************************
//******************************************************************************
CmdLineExec::CmdLineExec()
{
}
//******************************************************************************
//******************************************************************************
//******************************************************************************
void CmdLineExec::reset()
{
}
//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::execute(Ris::CmdLineCmd* aCmd)
{
   if(aCmd->isCmd("TP"    ))  executeTP      (aCmd);
   if(aCmd->isCmd("TEST"  ))  executeTest    (aCmd);
   if(aCmd->isCmd("GO1"   ))  executeGo1     (aCmd);
   if(aCmd->isCmd("GO2"   ))  executeGo2     (aCmd);
   if(aCmd->isCmd("GO3"   ))  executeGo3     (aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeTP(Ris::CmdLineCmd* aCmd)
{
   gTimerThread->mTPFlag=aCmd->argBool(1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeTest(Ris::CmdLineCmd* aCmd)
{
   gTimerThread->mTestCode=aCmd->argInt(1);
}


//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo1 (Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,100);
   aCmd->setArgDefault(2,50);

   int tCount = aCmd->argInt(1);
   int tSleep = aCmd->argInt(2);

   Ris::PeriodicTimeMarker tMarker;

   tMarker.initialize(tCount);

   for (int i = 0; i < tCount; i++)
   {
      tMarker.doStart();
      Ris::portableSleep(tSleep);
      tMarker.doStop();
   }

   if (tMarker.mStatistics.mEndOfPeriod || true)
   {
      Prn::print(Prn::ThreadRun1, "TEST %5d $$ %10.3f  %10.3f  %10.3f  %10.3f",
         tMarker.mStatistics.mSize,
         tMarker.mStatistics.mMean,
         tMarker.mStatistics.mStdDev,
         tMarker.mStatistics.mMinX,
         tMarker.mStatistics.mMaxX);
   }

}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo2 (Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo3 (Ris::CmdLineCmd* aCmd)
{
}



