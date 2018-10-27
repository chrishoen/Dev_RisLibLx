#include "stdafx.h"

#include "risThreadsProcess.h"
#include "someThreadParms.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize program resources.

void main_initialize(int argc,char** argv)
{
   // Enter process.
   Ris::Threads::enterProcessHigh();

   // Initialize print facility.
   Prn::resetPrint();
   Prn::useConsole(1);
   Prn::useConsole(2);
   Prn::initializePrint();

   // Initialize print filters.
   Prn::setFilter(Prn::ThreadInit1,     true);
   Prn::setFilter(Prn::ThreadRun1,      true);
   Prn::setFilter(Prn::ThreadRun2,      false);
   Prn::setFilter(Prn::ThreadRun3,      false);
   Prn::setFilter(Prn::ThreadRun4,      false);
   Prn::setFilter(Prn::View11,          true,  1);
   Prn::setFilter(Prn::View12,          false, 1);
   Prn::setFilter(Prn::View21,          true,  2);
   Prn::setFilter(Prn::View22,          false, 2);

   // Read parameters files.
   Some::gThreadParms.reset();
   Some::gThreadParms.readSection("default");

   // Done.
   Prn::print(0, "ThreadQCall Program************************************BEGIN");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Finalize program resources.

void main_finalize()
{
   Prn::print(0, "ThreadQCall Program************************************END");

   // Finalize print facility.
   Prn::finalizePrint();

   // Exit process.
   Ris::Threads::exitProcess();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
