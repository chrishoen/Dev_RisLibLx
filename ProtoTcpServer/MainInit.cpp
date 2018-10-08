#include "stdafx.h"

#include "risThreadsProcess.h"
#include "procoTcpSettings.h"

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

   ProtoComm::gTcpSettings.reset();
   ProtoComm::gTcpSettings.readSection("default");
   ProtoComm::gTcpSettings.readSection("TcpServer");
   ProtoComm::gTcpSettings.show();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Done.

   Prn::print(0, "ProtoCommTS Server***********************************BEGIN");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Finalize

void main_finalize()
{
   Prn::print(0,"ProtoCommTS Server***********************************END");

   // Close print.
   Prn::finalizePrint();

   // Exit process.
   Ris::Threads::exitProcess();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
