/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "my_functions.h"

#include "risCmdLineTables.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

CmdLineDoubleTable1D::CmdLineDoubleTable1D()
{
   mValues = 0;
   reset();
}

void CmdLineDoubleTable1D::reset()
{
   if (mValues) delete mValues;
   mValues = 0;

   mRows   = 0;
   mInitialized = false;
   mFirstCmd = true;
   mAlloc = 0;
   mIndex = 0;
   mValidFlag = false;
}

CmdLineDoubleTable1D::~CmdLineDoubleTable1D()
{
   if (mValues) delete mValues;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineDoubleTable1D::initialize(int aRows)
{
   reset();

   mRows  = aRows;
   mAlloc = mRows;

   mValues = new double[mAlloc];
   for (int i=0;i<mAlloc;i++) mValues[i]=0.0;

   mInitialized = true;
   mIndex = 0;
   mValidFlag = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

double& CmdLineDoubleTable1D::e (int aRow)
{
   return mValues[aRow];
}

double& CmdLineDoubleTable1D::operator() (int aRow)
{
   return mValues[aRow];
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineDoubleTable1D::show(char* aLabel)
{
   if (!mValidFlag)
   {
      printf("Table1D %s NOT VALID\n", aLabel);
      return;
   }

   char tLabel[30];

   if (aLabel==0)
   {
      strcpy(tLabel,"CmdLineDoubleTable1D");
   }
   else
   {
      strncpy(tLabel,aLabel,30);
   }

   for (int i=0;i<mRows;i++) printf("%s  %d  %10.8f\n", tLabel,i, e(i));
   printf("\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Execute, overload used to read from a command line file. This is called
// for each line in the corresponding table section of the file. It parses 
// the file command line to read table values
//
// For each command line, such as:
//
// "Command Arg1 Arg2 Arg3"
// 
// The number of arguments is 3
// Command is at arg[0]
// Arg1    is at arg[1]
// Arg2    is at arg[2]
// Arg3    is at arg[3]


void CmdLineDoubleTable1D::execute(Ris::CmdLineCmd* aCmd)
{
   // Argument variables
   int tNumUsableArg = 0;
   int tArgOffset    = 0;

   // If this is the first command line for the table
   if (mFirstCmd)
   {
      mFirstCmd = false;
      // If already initialized in parent code,
      // size is already specified
      if (mInitialized)
      {
         // Start using available arguments
         tNumUsableArg = aCmd->numArg();
         tArgOffset    = 1;
      }
      // If not already initialized in parent code,
      // size is specified by first argument
      else
      {
         // Initialize
         int tRows = aCmd->argInt(1);
         initialize(tRows);
         // Start using available arguments
         tNumUsableArg = aCmd->numArg() - 1;
         tArgOffset    = 2;
      }
      // Push this executive onto the nested executive stack
      nestedPush(aCmd, this);
   }
   // If this is not the first command line for the table
   else
   {
      // Use all available arguments, including arg[0],
      // beacuse there will be no command in arg[0] for this
      // command line
      tNumUsableArg = aCmd->numArg() + 1;
      tArgOffset = 0;
   }

   // Read values from arguments into values array
   for (int i = 0; i < tNumUsableArg; i++)
   {
      // Copy argument
      mValues[mIndex++] = aCmd->argDouble(tArgOffset + i);
      // If values array if full then done
      if (mIndex == mAlloc)
      {
         // Set valid
         mValidFlag = true;
         // Pop this executive off of the nested executive stack
         nestedPop(aCmd);
         return;
      }
   }
}

void CmdLineDoubleTable1D::reexecute(Ris::CmdLineCmd* aCmd)
{
   mFirstCmd = true;
   mIndex = 0;
   mValidFlag = false;

   execute(aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

CmdLineDoubleTable2D::CmdLineDoubleTable2D()
{
   mValues = 0;
   reset();
}

void CmdLineDoubleTable2D::reset()
{
   if (mValues) delete mValues;
   mValues = 0;

   mRows   = 0;
   mCols   = 0;
   mInitialized = false;
   mFirstCmd = true;
   mAlloc = 0;
   mIndex = 0;
   mValidFlag = false;
}
CmdLineDoubleTable2D::~CmdLineDoubleTable2D()
{
   if (mValues) delete mValues;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineDoubleTable2D::initialize(int aRows,int aCols)
{
   reset();

   mRows   = aRows;
   mCols   = aCols;
   mAlloc  = mRows*mCols;

   mValues = new double[mAlloc];
   for (int i=0;i<mAlloc;i++) mValues[i]=0.0;

   mInitialized = true;
   mIndex = 0;
   mValidFlag = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

double& CmdLineDoubleTable2D::e (int aRow,int aCol)
{
   int tIndex = (aRow)*mCols + (aCol);
   return mValues[tIndex];
}

double& CmdLineDoubleTable2D::operator() (int aRow,int aCol)
{
   int tIndex = (aRow)*mCols + (aCol);
   return mValues[tIndex];
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineDoubleTable2D::show(char* aLabel)
{
   if (!mValidFlag)
   {
      printf("Table2D %s NOT VALID\n", aLabel);
      return;
   }

   char tLabel[30];

   if (aLabel==0)
   {
      strcpy(tLabel,"CmdLineDoubleTable2D");
   }
   else
   {
      strncpy(tLabel,aLabel,30);
   }

   for (int i=0;i<mRows;i++)
   {
      printf("%s ",tLabel);
      for (int j=0;j<mCols;j++)
      {
         printf("%12.8f ",e(i,j));
      }
      printf("\n");
   }
   printf("\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Execute, overload used to read from a command line file. This is called
// for each line in the corresponding table section of the file. It parses 
// the file command line to read table values
//
// For each command line, such as:
//
// "Command Arg1 Arg2 Arg3"
// 
// The number of arguments is 3
// Command is at arg[0]
// Arg1    is at arg[1]
// Arg2    is at arg[2]
// Arg3    is at arg[3]


void CmdLineDoubleTable2D::reexecute(Ris::CmdLineCmd* aCmd)
{
   mFirstCmd = true;
   mIndex = 0;
   mValidFlag = false;

   execute(aCmd);
}

void CmdLineDoubleTable2D::execute(Ris::CmdLineCmd* aCmd)
{
   // Argument variables
   int tNumUsableArg = 0;
   int tArgOffset    = 0;

   // If this is the first command line for the table
   if (mFirstCmd)
   {
      mFirstCmd = false;
      // If already initialized in parent code,
      // size is already specified
      if (mInitialized)
      {
         // Start using available arguments
         tNumUsableArg = aCmd->numArg();
         tArgOffset    = 1;
      }
      // If not already initialized in parent code,
      // size is specified by first argument
      else
      {
         // Initialize
         int tRows = aCmd->argInt(1);
         int tCols = aCmd->argInt(2);
         initialize(tRows,tCols);
         // Start using available arguments
         tNumUsableArg = aCmd->numArg() - 2;
         tArgOffset    = 3;
      }
      // Push this executive onto the nested executive stack
      nestedPush(aCmd, this);
   }
   // If this is not the first command line for the table
   else
   {
      // Use all available arguments, including arg[0],
      // beacuse there will be no command in arg[0] for this
      // command line
      tNumUsableArg = aCmd->numArg() + 1;
      tArgOffset = 0;
   }

   // Read values from arguments into values array
   for (int i = 0; i < tNumUsableArg; i++)
   {
      // Copy argument
      mValues[mIndex++] = aCmd->argDouble(tArgOffset + i);
      // If values array if full then done
      if (mIndex == mAlloc)
      {
         // Set valid
         mValidFlag = true;
         // Pop this executive off of the nested executive stack
         nestedPop(aCmd);
         return;
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

CmdLineIntTable1D::CmdLineIntTable1D()
{
   mValues = 0;
   reset();
}

void CmdLineIntTable1D::reset()
{
   if (mValues) delete mValues;
   mValues = 0;

   mRows   = 0;
   mInitialized = false;
   mFirstCmd = true;
   mAlloc = 0;
   mIndex = 0;
   mValidFlag = false;
}

CmdLineIntTable1D::~CmdLineIntTable1D()
{
   if (mValues) delete mValues;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineIntTable1D::initialize(int aRows)
{
   reset();

   mRows  = aRows;
   mAlloc = mRows;

   mValues = new int[mAlloc];
   for (int i=0;i<mAlloc;i++) mValues[i]=0;

   mInitialized = true;
   mIndex = 0;
   mValidFlag = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

int& CmdLineIntTable1D::e (int aRow)
{
   return mValues[aRow];
}

int& CmdLineIntTable1D::operator() (int aRow)
{
   return mValues[aRow];
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineIntTable1D::show(char* aLabel)
{
   if (!mValidFlag)
   {
      printf("Table1D %s NOT VALID\n", aLabel);
      return;
   }

   char tLabel[30];

   if (aLabel==0)
   {
      strcpy(tLabel,"CmdLineIntTable1D");
   }
   else
   {
      strncpy(tLabel,aLabel,30);
   }

   for (int i=0;i<mRows;i++) printf("%s  %d  %10d\n", tLabel,i, e(i));
   printf("\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Execute, overload used to read from a command line file. This is called
// for each line in the corresponding table section of the file. It parses 
// the file command line to read table values
//
// For each command line, such as:
//
// "Command Arg1 Arg2 Arg3"
// 
// The number of arguments is 3
// Command is at arg[0]
// Arg1    is at arg[1]
// Arg2    is at arg[2]
// Arg3    is at arg[3]


void CmdLineIntTable1D::execute(Ris::CmdLineCmd* aCmd)
{
   // Argument variables
   int tNumUsableArg = 0;
   int tArgOffset    = 0;

   // If this is the first command line for the table
   if (mFirstCmd)
   {
      mFirstCmd = false;
      // If already initialized in parent code,
      // size is already specified
      if (mInitialized)
      {
         // Start using available arguments
         tNumUsableArg = aCmd->numArg();
         tArgOffset    = 1;
      }
      // If not already initialized in parent code,
      // size is specified by first argument
      else
      {
         // Initialize
         int tRows = aCmd->argInt(1);
         initialize(tRows);
         // Start using available arguments
         tNumUsableArg = aCmd->numArg() - 1;
         tArgOffset    = 2;
      }
      // Push this executive onto the nested executive stack
      nestedPush(aCmd, this);
   }
   // If this is not the first command line for the table
   else
   {
      // Use all available arguments, including arg[0],
      // beacuse there will be no command in arg[0] for this
      // command line
      tNumUsableArg = aCmd->numArg() + 1;
      tArgOffset = 0;
   }

   // Read values from arguments into values array
   for (int i = 0; i < tNumUsableArg; i++)
   {
      // Copy argument
      mValues[mIndex++] = aCmd->argInt(tArgOffset + i);
      // If values array if full then done
      if (mIndex == mAlloc)
      {
         // Set valid
         mValidFlag = true;
         // Pop this executive off of the nested executive stack
         nestedPop(aCmd);
         return;
      }
   }
}

void CmdLineIntTable1D::reexecute(Ris::CmdLineCmd* aCmd)
{
   mFirstCmd = true;
   mIndex = 0;
   mValidFlag = false;

   execute(aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

CmdLineIntTable2D::CmdLineIntTable2D()
{
   mValues = 0;
   reset();
}

void CmdLineIntTable2D::reset()
{
   if (mValues) delete mValues;
   mValues = 0;

   mRows   = 0;
   mCols   = 0;
   mInitialized = false;
   mFirstCmd = true;
   mAlloc = 0;
   mIndex = 0;
   mValidFlag = false;
}
CmdLineIntTable2D::~CmdLineIntTable2D()
{
   if (mValues) delete mValues;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineIntTable2D::initialize(int aRows,int aCols)
{
   reset();

   mRows   = aRows;
   mCols   = aCols;
   mAlloc  = mRows*mCols;

   mValues = new int[mAlloc];
   for (int i=0;i<mAlloc;i++) mValues[i]=0;

   mInitialized = true;
   mIndex = 0;
   mValidFlag = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

int& CmdLineIntTable2D::e (int aRow,int aCol)
{
   int tIndex = (aRow)*mCols + (aCol);
   return mValues[tIndex];
}

int& CmdLineIntTable2D::operator() (int aRow,int aCol)
{
   int tIndex = (aRow)*mCols + (aCol);
   return mValues[tIndex];
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineIntTable2D::show(char* aLabel)
{
   if (!mValidFlag)
   {
      printf("Table2D %s NOT VALID\n", aLabel);
      return;
   }

   char tLabel[30];

   if (aLabel==0)
   {
      strcpy(tLabel,"CmdLineIntTable2D");
   }
   else
   {
      strncpy(tLabel,aLabel,30);
   }

   for (int i=0;i<mRows;i++)
   {
      printf("%s ",tLabel);
      for (int j=0;j<mCols;j++)
      {
         printf("%10d",e(i,j));
      }
      printf("\n");
   }
   printf("\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Execute, overload used to read from a command line file. This is called
// for each line in the corresponding table section of the file. It parses 
// the file command line to read table values
//
// For each command line, such as:
//
// "Command Arg1 Arg2 Arg3"
// 
// The number of arguments is 3
// Command is at arg[0]
// Arg1    is at arg[1]
// Arg2    is at arg[2]
// Arg3    is at arg[3]


void CmdLineIntTable2D::reexecute(Ris::CmdLineCmd* aCmd)
{
   mFirstCmd = true;
   mIndex = 0;
   mValidFlag = false;

   execute(aCmd);
}

void CmdLineIntTable2D::execute(Ris::CmdLineCmd* aCmd)
{
   // Argument variables
   int tNumUsableArg = 0;
   int tArgOffset    = 0;

   // If this is the first command line for the table
   if (mFirstCmd)
   {
      mFirstCmd = false;
      // If already initialized in parent code,
      // size is already specified
      if (mInitialized)
      {
         // Start using available arguments
         tNumUsableArg = aCmd->numArg();
         tArgOffset    = 1;
      }
      // If not already initialized in parent code,
      // size is specified by first argument
      else
      {
         // Initialize
         int tRows = aCmd->argInt(1);
         int tCols = aCmd->argInt(2);
         initialize(tRows,tCols);
         // Start using available arguments
         tNumUsableArg = aCmd->numArg() - 2;
         tArgOffset    = 3;
      }
      // Push this executive onto the nested executive stack
      nestedPush(aCmd, this);
   }
   // If this is not the first command line for the table
   else
   {
      // Use all available arguments, including arg[0],
      // beacuse there will be no command in arg[0] for this
      // command line
      tNumUsableArg = aCmd->numArg() + 1;
      tArgOffset = 0;
   }

   // Read values from arguments into values array
   for (int i = 0; i < tNumUsableArg; i++)
   {
      // Copy argument
      mValues[mIndex++] = aCmd->argInt(tArgOffset + i);
      // If values array if full then done
      if (mIndex == mAlloc)
      {
         // Set valid
         mValidFlag = true;
         // Pop this executive off of the nested executive stack
         nestedPop(aCmd);
         return;
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

CmdLineStringTable::CmdLineStringTable()
{
   mRows   = 0;
   mStringStore = 0;

   mInitialized = false;
   mFirstCmd = true;
   mAlloc = 0;
   mIndex = 0;
   mValidFlag = false;
}

CmdLineStringTable::~CmdLineStringTable()
{
   if (mStringStore) delete mStringStore;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineStringTable::initialize(
   int     aRows,
   int     aStringSize)
{
   mRows       = aRows;
   mStringSize = aStringSize;
   int tAlloc = mRows*mStringSize;
   mStringStore = new char[tAlloc];
   mStringPtr   = new char*[mRows];

   for (int i = 0; i < mRows; i++)
   {
      mStringPtr[i] = mStringStore + i*mStringSize;
      my_strncpy(mStringPtr[i], "NULL",10);
   }

   mInitialized = true;
   mAlloc = tAlloc;
   mIndex = 0;
   mValidFlag = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

char* CmdLineStringTable::e (int aRow)
{
   return mStringPtr[aRow];
}

char* CmdLineStringTable::operator() (int aRow)
{
   return mStringPtr[aRow];
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineStringTable::show(char* aLabel)
{
   if (!mValidFlag)
   {
      printf("CmdLineStringTable %s NOT VALID\n", aLabel);
      return;
   }

   char tLabel[30];

   if (aLabel==0)
   {
      strcpy(tLabel,"CmdLineStringTable");
   }
   else
   {
      strncpy(tLabel,aLabel,30);
   }

   for (int i=0;i<mRows;i++) printf("%s  %d  %s\n", tLabel,i, e(i));
   printf("\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Execute, overload used to read from a command line file. This is called
// for each line in the corresponding table section of the file. It parses 
// the file command line to read table values
//
// For each command line, such as:
//
// "Command Arg1 Arg2 Arg3"
// 
// The number of arguments is 3
// Command is at arg[0]
// Arg1    is at arg[1]
// Arg2    is at arg[2]
// Arg3    is at arg[3]


void CmdLineStringTable::execute(Ris::CmdLineCmd* aCmd)
{
   // Argument variables
   int tNumUsableArg = 0;
   int tArgOffset    = 0;

   // If this is the first command line for the table
   if (mFirstCmd)
   {
      mFirstCmd = false;
      // If already initialized in parent code,
      // size is already specified
      if (mInitialized)
      {
         // Start using available arguments
         tNumUsableArg = aCmd->numArg();
         tArgOffset    = 1;
      }
      // If not already initialized in parent code,
      // size is specified by first argument
      else
      {
         // Initialize
         int tRows       = aCmd->argInt(1);
         int tStringSize = aCmd->argInt(2);
         initialize(tRows,tStringSize);
         // Start using available arguments
         tNumUsableArg = aCmd->numArg() - 1;
         tArgOffset    = 2;
      }
      // Push this executive onto the nested executive stack
      nestedPush(aCmd, this);
   }
   // If this is not the first command line for the table
   else
   {
      // Use all available arguments, including arg[0],
      // beacuse there will be no command in arg[0] for this
      // command line
      tNumUsableArg = aCmd->numArg() + 1;
      tArgOffset = 0;
   }

   // Read values from arguments into values array
   for (int i = 0; i < tNumUsableArg; i++)
   {
      // Copy argument
      aCmd->copyArgString(tArgOffset + i,mStringPtr[mIndex++],mStringSize);
      // If values array if full then done
      if (mIndex == mRows)
      {
         // Set valid
         mValidFlag = true;
         // Pop this executive off of the nested executive stack
         nestedPop(aCmd);
         return;
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

