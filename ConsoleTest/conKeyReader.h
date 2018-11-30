#pragma once

/*==============================================================================
KeyReader Services.
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
// This is a record that describes single keyboard inputs.

class KeyRecord
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   int  mCode;
   char mChar;
   bool mIsPrintable;
   bool mIsShift;
   bool mIsControl;
   bool mIsAlt;
   bool mIsControlShift;
   bool mIsAltShift;
   bool mIsFunction;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   KeyRecord();
   void reset();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constants.

static const int cKey_NoCode      = 0;
static const int cKey_Printable   = 1;
static const int cKey_Control     = 2;
static const int cKey_Function    = 3;

static const int cKey_Enter       = 1000;
static const int cKey_BackSpace   = 1001;
static const int cKey_Delete      = 1002;
static const int cKey_LeftArrow   = 1003;
static const int cKey_RightArrow  = 1004;
static const int cKey_UpArrow     = 1005;
static const int cKey_DownArrow   = 1006;
static const int cKey_Home        = 1007;
static const int cKey_End         = 1008;
static const int cKey_Escape      = 1009;

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This class provides global program console i/o facility.

class KeyReader
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Print flag.
   bool mPF;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   KeyReader();

   // Initialize.
   void initialize();
   void finalize();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Read a single console keyboard input. Return it in the input record.
   void readKey(KeyRecord* aRecord);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Read a single character from the console.
   int readOne();

   // Return the number of characters that are available to be read.
   int getReadAvailable();

   // Write a single character to the console.
   void writeOne(char aChar);

   // Write a string to the console.
   void writeString(char* aString);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   void onKey_ControlC   (int aKeyIn, KeyRecord* aRecord);
   void onKey_Enter      (int aKeyIn, KeyRecord* aRecord);
   void onKey_BackSpace  (int aKeyIn, KeyRecord* aRecord);
   void onKey_Printable  (int aKeyIn, KeyRecord* aRecord);
   void onKey_Control    (int aKeyIn, KeyRecord* aRecord);
   bool onKey_Escape(int aKeyIn, KeyRecord* aRecord);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global singular instance.

#ifdef _CONKEYREADER_CPP_
          KeyReader gKeyReader;
#else
   extern KeyReader gKeyReader;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

