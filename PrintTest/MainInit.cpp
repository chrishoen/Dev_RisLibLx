#include "stdafx.h"

#include "risThreadsProcess.h"

#include "someTimerThread.h"
using namespace Some;

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize

void main_initialize(int argc,char** argv)
{
   // Enter process
   Ris::Threads::enterProcessHigh();

   // Initialize print facility
   Prn::resetPrint();
// Prn::useConsole(0);
   Prn::useConsole(1);
   Prn::initializePrint();

   // Initialize print filters
   Prn::setFilter(Prn::SocketInitS1,    true);
   Prn::setFilter(Prn::SocketInitS2,    false);
   Prn::setFilter(Prn::SocketRxRunS1,   false);
   Prn::setFilter(Prn::SocketRxRunS2,   false);
   Prn::setFilter(Prn::SocketTxRunS1,   false);
   Prn::setFilter(Prn::SocketTxRunS2,   false);
   Prn::setFilter(Prn::SocketErrorS1,   true);
   Prn::setFilter(Prn::SocketErrorS2,   false);

   Prn::setFilter(Prn::SocketInitT1,    true);
   Prn::setFilter(Prn::SocketInitT2,    false);
   Prn::setFilter(Prn::SocketRxRunT1,   false);
   Prn::setFilter(Prn::SocketRxRunT2,   false);
   Prn::setFilter(Prn::SocketTxRunT1,   false);
   Prn::setFilter(Prn::SocketTxRunT2,   false);
   Prn::setFilter(Prn::SocketErrorT1,   true);
   Prn::setFilter(Prn::SocketErrorT2,   true);

   Prn::setFilter(Prn::ThreadRun1,      true);
   Prn::setFilter(Prn::ThreadRun2,      false);
   Prn::setFilter(Prn::ThreadRun3,      true);
   Prn::setFilter(Prn::ThreadRun4,      true);

   Prn::setFilter(Prn::ProcRun1,        true);
   Prn::setFilter(Prn::ProcRun2,        true   ,1);
   Prn::setFilter(Prn::ProcRun3,        false);
   Prn::setFilter(Prn::ProcRun4,        true);

   Prn::setFilter(Prn::ViewRun1,        true   ,1);
   Prn::setFilter(Prn::ViewRun2,        true   ,1);
   Prn::setFilter(Prn::ViewRun3,        false  ,1);
   Prn::setFilter(Prn::ViewRun4,        true   ,1);

   Log::reset();
   Log::openFile(0,"LogPrintTest.txt");

   Prn::print(0,"Program PrintTest***********************************BEGIN");
   Log::write(0,"Program PrintTest***********************************BEGIN");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Finalize

void main_finalize()
{
   Prn::print(0,"Program PrintTest***********************************END");
   Log::write(0,"Program PrintTest***********************************END");
   Log::closeAllFiles();

   // Close print
   Prn::finalizePrint();

   // Exit process
   Ris::Threads::exitProcess();

}

