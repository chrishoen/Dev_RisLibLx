#include "stdafx.h"

#include "MainInit.h"

#include "conTestRaw.h"
#include "conInputReader.h"


//******************************************************************************
//******************************************************************************
//******************************************************************************

int main(int argc,char** argv)
{
   main_initialize(argc,argv);

   if (true)
   {
      Con::gInputReader.initialize();
      Con::gInputReader.doTestLoop1();
      Con::gInputReader.finalize();
   }
   else
   {
      Con::initializeRaw();
      Con::doTestLoopRaw1();
      Con::finalizeRaw();
   }

   main_finalize();
   return 0;
}
