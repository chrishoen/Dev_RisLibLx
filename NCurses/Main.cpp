#include "stdafx.h"

#include "MainInit.h"

#include "conConsole.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

int main(int argc,char** argv)
{
   main_initialize(argc,argv);

   int tCode = 1;
   if (argc == 2)
   {
      if (strcmp(argv[1], "test1") == 0) tCode = 1;
      if (strcmp(argv[1], "test2") == 0) tCode = 2;
      printf("code %d\n", tCode);
   }

   if (tCode == 1)
   {
      Con::gConsole.initialize();
      Con::gConsole.doTestLoop1();
      Con::gConsole.finalize();
   }
   else if (tCode == 2)
   {
      Con::gConsole.initialize();
      Con::gConsole.doTestLoop2();
      Con::gConsole.finalize();
   }

   main_finalize();
   return 0;
}
