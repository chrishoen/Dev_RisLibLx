#include "stdafx.h"

#include "tsThreadServices.h"
#include "risThreadsProcess.h"
#include "someThreadParms.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize program resources.

void main_initialize(int argc,char** argv)
{
   printf("ThreadTest Program*********************************************BEGIN\n");
   printf("ThreadTest Program*********************************************BEGIN\n");
   printf("ThreadTest Program*********************************************BEGIN\n\n");

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize thread services.

   TS::reset();
   TS::setProgramName("ThreadTest");
   TS::setProgramPrintLevel(TS::PrintLevel(3, 3));
   TS::initialize();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Enter process.

   Ris::Threads::enterProcessHigh();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize print facility.

   // Initialize print.
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

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read parameters files.

   // Read parameters files.
   Some::gThreadParms.reset();
   Some::gThreadParms.readSection("default");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Finalize program resources.

void main_finalize()
{
   // Finalize print facility.
   Prn::finalizePrint();

   // Exit process.
   Ris::Threads::exitProcess();

   // Finalize thread services.
   TS::finalize();

   // Done.
   printf("\n");
   printf("ThreadTest Program*********************************************END\n\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
