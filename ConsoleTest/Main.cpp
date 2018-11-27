#include "stdafx.h"

#include "MainInit.h"

#include "conInputReader.h"


//******************************************************************************
//******************************************************************************
//******************************************************************************

int main(int argc,char** argv)
{
   main_initialize(argc,argv);

   Con::gInputReader.initialize();
   Con::gInputReader.doTestLoop1();
   Con::gInputReader.finalize();

   main_finalize();
   return 0;
}
