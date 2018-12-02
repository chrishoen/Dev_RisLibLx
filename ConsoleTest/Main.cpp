#include "stdafx.h"

#include "MainInit.h"

#include "conInputReader_linux.h"
#include "conInputTest_linux.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

int main(int argc,char** argv)
{
   main_initialize(argc,argv);

   int tCode = 0;
   if (argc == 2)
   {
      if (strcmp(argv[1], "read") == 0) tCode = 0;
      if (strcmp(argv[1], "test1") == 0) tCode = 1;
      if (strcmp(argv[1], "test2") == 0) tCode = 2;
      printf("code %d\n", tCode);
   }

   if (tCode == 0)
   {
      Con::gInputReader.initialize();
      Con::gInputReader.doTestLoop1();
      Con::gInputReader.finalize();
   }
   else if (tCode == 1)
   {
      Con::gInputTest.initialize();
      Con::gInputTest.doTestLoop1();
      Con::gInputTest.finalize();
   }
   else if (tCode == 2)
   {
      Con::gInputTest.initialize();
      Con::gInputTest.doTestLoop2();
      Con::gInputTest.finalize();
   }

   main_finalize();
   return 0;
}
