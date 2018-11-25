#include "stdafx.h"

#include "MainInit.h"
#include "risCmdLineConsole.h"
#include "CmdLineExec.h"

#include "conTestRaw.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

int main(int argc,char** argv)
{
   main_initialize(argc,argv);

   Con::initializeRaw();
   Con::doTestLoopRaw();
   Con::finalizeRaw();

   main_finalize();
   return 0;
}
