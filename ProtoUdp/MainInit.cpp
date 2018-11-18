#include "stdafx.h"

#include "tsThreadServices.h"
#include "risThreadsProcess.h"
#include "procoUdpSettings.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize

void main_initialize(int argc,char** argv)
{
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
   Prn::initializePrint();

   // Initialize print filters.
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

   Prn::setFilter(Prn::ThreadInit1,     true);
   Prn::setFilter(Prn::ThreadInit1,     true);
   Prn::setFilter(Prn::ThreadRun1,      true);
   Prn::setFilter(Prn::ThreadRun2,      false);
   Prn::setFilter(Prn::ThreadRun3,      false);
   Prn::setFilter(Prn::ThreadRun4,      false);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read parameters files.

   if (argc == 2)
   {
      ProtoComm::gUdpSettings.reset();
      ProtoComm::gUdpSettings.readSection("default");
      ProtoComm::gUdpSettings.readSection(argv[1]);
      ProtoComm::gUdpSettings.show();
   }
   else
   {
      ProtoComm::gUdpSettings.reset();
      ProtoComm::gUdpSettings.readSection("default");
      ProtoComm::gUdpSettings.readSection("UdpPeer1");
      ProtoComm::gUdpSettings.show();
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize thread services.

   char tProgramName[100];
   sprintf(tProgramName, "ProtoUdp%d", ProtoComm::gUdpSettings.mMyAppNumber);

   TS::reset();
   TS::setProgramName(tProgramName);
   TS::setProgramPrintLevel(TS::PrintLevel(2,3));
   TS::initialize();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Done.

   Prn::print(0, "ProtoUdp Program************************************BEGIN %s", argv[1]);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Finalize.

void main_finalize()
{
   Prn::print(0,"ProtoUdp Program*************************************END");

   // Finalize print facility.
   Prn::finalizePrint();

   // Exit process.
   Ris::Threads::exitProcess();

   // Finalize thread services.
   TS::finalize();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
