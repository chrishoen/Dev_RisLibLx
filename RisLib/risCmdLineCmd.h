#pragma once

/*==============================================================================
Command line command. 
==============================================================================*/

//******************************************************************************
namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This encapsulates a single command line command.
// It is given a string of the form "command argument1 argumant2.." where
// the arguments can hold values for primitive data types such as int,bool,char*,
// double. An example command line command is the string "transmit true 101 1.1".
// This class provides parsing of the command line and methods to extract the
// commandand the arguments. It is passed an input string and provides methods
// that are used to extract the command and the arguments. 

class CmdLineExecNestedAnchor;

class  CmdLineCmd
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   enum {MaxNumOfArgs =  20};
   enum {MaxArgSize   = 100};

   // Copy of the input command line.
   char mCommandLine[1000];

   // Argument delimiters.
   char mDelimiters[10];

   // Command line string argument pointer.
   // mArgPtr[0] is the command, mArgPtr[1] is the first argument ...
   char*  mArgPtr   [MaxNumOfArgs+1];
   // Command line string argument storage.
   // mArgPtr[n] points to mArgStore[n].
   char   mArgStore[MaxNumOfArgs + 1][MaxArgSize];
   // The whole set of arguments, all chars afer the command.
   char   mArgWhole[MaxArgSize*10];
   // True if corresponding argument exists
   bool   mArgFlag  [MaxNumOfArgs+1];
   // Number of arguments
   int    mArgNum;
   // True if an isCmd returned true   
   bool   mGoodCmd;
   // True if struct is valid
   bool   mValidFlag;

   // Nested anchor, used for processing nested records. This is piggybacked 
   // onto a command by a command line processor so that it can be used by a 
   // command line executive.
   CmdLineExecNestedAnchor* mNestedAnchor;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Intrastructure.

   // Constructor
   CmdLineCmd();
   CmdLineCmd(const char* aCommandLine);
   void reset();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Input a command line string.

   // Parse a command line string into the command object.
   // This inputs a command line string and parses it into the command and
   // argument members which can then be accessed via the extraction methods.
   void parseCmdLine (const char* aCommandLine);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods, command extraction.

   // Returns the number of arguments
   int  numArg ();

   // Returns true if aString is equal to the command
   bool isCmd (const char* aString);

   // Returns true if no calls to isCmd returned true
   bool isBadCmd ();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods, argument extraction.

   // Return an argument from the command line.
   // These are given the argument index, such that argInt(1) returns the
   // first argument as an integer and argInt(2) returns the second argument.
   int      argInt        (int aArgIndex);
   unsigned argUnsigned   (int aArgIndex);
   bool     argBool       (int aArgIndex);
   double   argDouble     (int aArgIndex);
   double   argAngle      (int aArgIndex);
   char*    argString     (int aArgIndex);

   // Test argument against a string.
   // It compares the argument with the string.
   bool isArgString   (int aArgIndex,const char* aString);

   // Copy an argument into a string
   void copyArgString (int aArgIndex,char* aString,int aMaxSize);

   // Return true if the argument is a number.
   bool isArgNumber(int aArgIndex);

   // Return a pointer to the whole argumant string.
   char*    argWhole();

   // Copy the whole argument into a string
   void copyArgWhole(char* aString, int aMaxSize);

   // Set defaults for arguments from the command line.
   // These are used as defaults if no arguments are entered.
   void setArgDefault         (int argIndex,  int      aValue);
   void setArgDefaultUnsigned (int aArgIndex, unsigned aValue);
   void setArgDefault         (int argIndex,  bool     aValue);
   void setArgDefault         (int argIndex,  double   aValue);
   void setArgDefault         (int argIndex,  char*    aString);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

