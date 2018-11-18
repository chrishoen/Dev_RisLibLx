#include "stdafx.h"

#include "tsThreadServices.h"
#include "risThreadsProcess.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize program resources.

void main_initialize(int argc,char** argv)
{
   printf("TimerThreadTest Program****************************************BEGIN\n");
   printf("TimerThreadTest Program****************************************BEGIN\n");
   printf("TimerThreadTest Program****************************************BEGIN\n\n");

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize thread services.

   TS::reset();
   TS::setProgramName("Timer");
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
   Prn::initializePrint();

   // Initialize print filters.
   Prn::setFilter(Prn::ThreadInit1,     true);
   Prn::setFilter(Prn::ThreadRun1,      true);
   Prn::setFilter(Prn::ThreadRun2,      false);
   Prn::setFilter(Prn::ThreadRun3,      false);
   Prn::setFilter(Prn::ThreadRun4,      false);
   Prn::setFilter(Prn::View11,          true,  1);
   Prn::setFilter(Prn::View12,          false, 1);
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
   printf("TimerThreadTest Program****************************************END\n\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
