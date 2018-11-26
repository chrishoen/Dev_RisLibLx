#include "stdafx.h"

#include "MainInit.h"

#include "conStringReader.h"


//******************************************************************************
//******************************************************************************
//******************************************************************************

int main(int argc,char** argv)
{
   main_initialize(argc,argv);

   Con::gStringReader.initialize();
   Con::gStringReader.doTestLoop1();
   Con::gStringReader.finalize();

   main_finalize();
   return 0;
}
