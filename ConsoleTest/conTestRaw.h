#pragma once

/*==============================================================================
Console Services. Read and write low level input and output raw test.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Con
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// These are a set of functions that provide low level console input and
// output raw tests. They are used to observe the raw key codes.

   void initializeRaw();
   void finalizeRaw();

   // Run a test loop that prints raw key codes.
   void doTestLoopRaw();
   void doTest1();

   // Raw input and output.
   int  readOneRaw();
   void writeOneRaw(int aChar);

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

