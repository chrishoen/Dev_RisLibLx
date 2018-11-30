#pragma once

/*==============================================================================
InputReader Services.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "conKeyReader.h"
#include "conInputHistory.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Con
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This class provides global program console i/o facility.

class InputReader
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants.

   static const int cMaxStringSize = 400;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Keyboard input.
   KeyRecord mKeyIn;

   // Cursor postion.
   int mCursor;

   // Input string.
   char mInputString[cMaxStringSize];
   int mInputLength;

   // Output string.
   char mOutputString[cMaxStringSize];

   // Input string history.
   InputHistory mInputHistory;

   // Print flag.
   bool mPF;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   InputReader();
   void resetVariables();

   // Initialize.
   void initialize();
   void finalize();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Run test loop.
   void doTestLoop1();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   void onKey_Enter();
   void onKey_BackSpace();
   void onKey_Delete();
   void onKey_LeftArrow();
   void onKey_RightArrow();
   void onKey_UpArrow();
   void onKey_DownArrow();
   void onKey_Home();
   void onKey_End();
   void onKey_Insert();
   void onKey_PageUp();
   void onKey_PageDown();
   void onKey_Printable();
   void onKey_Control();
   void onKey_Function();
   void onKey_Escape();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Write the input string to the console output and position the cursor.
   // This takes mInputString and mCursor and echos to the console output
   // appropriately.
   void echoInput();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global singular instance.

#ifdef _CONSTRINGREADER_CPP_
          InputReader gInputReader;
#else
   extern InputReader gInputReader;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

