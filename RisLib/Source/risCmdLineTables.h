#pragma once
/*==============================================================================

Table classes that can be used by the command line file classes.
One or two dimensional tables classes of doubles.
One ddimensional talble of string.

==============================================================================*/

#include "risCmdLineExec.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Table, 1 dimensional, indexed by 0..R-1
  
class CmdLineDoubleTable1D : public Ris::BaseCmdLineExec
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Vector components.
   double* mValues;
   int     mRows;

   // True if initialized.
   bool mInitialized;
   // True if this is the first command line.
   bool mFirstCmd;
   // Number of doubles allocated in mValues.
   int  mAlloc;
   // Current index into mValues.
   int  mIndex;
   // True if all values were read in correctly.
   bool mValidFlag;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Intrastructure.

   // Constructor.
   CmdLineDoubleTable1D();
  ~CmdLineDoubleTable1D();
   void reset();

   // Initialize and allocate memory for the table.
   void initialize(int aRows);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Access components.
   double& e(int aRow);
   double& operator()(int aRow);
   void show(char* aLabel=0);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Execute, overload used to read from a command line file. This is called
   // for each line in the corresponding table section of the file. It parses 
   // the file command line to read table values
   void execute(Ris::CmdLineCmd* aCmd);

   // Execute again. reinitializes and calls execute.
   void reexecute(Ris::CmdLineCmd* aCmd);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Table, 2 dimensional, indexed by 0..R-1,0..C-1

class CmdLineDoubleTable2D : public Ris::BaseCmdLineExec
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Matrix components.
   double* mValues;
   int     mRows;
   int     mCols;

   // True if initialized.
   bool mInitialized;
   // True if this is the first command line.
   bool mFirstCmd;
   // Number of doubles allocated in mValues.
   int  mAlloc;
   // Current index into mValues.
   int  mIndex;
   // True if all values were read in correctly.
   bool mValidFlag;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Intrastructure.

   // Constructor.
   CmdLineDoubleTable2D();
  ~CmdLineDoubleTable2D();
   void reset();

   // Initialize and allocate memory for the table.
   void initialize(int aRows,int aCols);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Access components.
   double& e(int aRow,int aCol);
   double& operator()(int aRow,int aCol);
   void show(char* aLabel=0);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Execute, overload used to read from a command line file. This is called
   // for each line in the corresponding table section of the file. It parses 
   // the file command line to read table values
   void execute(Ris::CmdLineCmd* aCmd);

   // Execute again. reinitializes and calls execute.
   void reexecute(Ris::CmdLineCmd* aCmd);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Table, 1 dimensional, indexed by 0..R-1
  
class CmdLineIntTable1D : public Ris::BaseCmdLineExec
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Vector components.
   int* mValues;
   int     mRows;

   // True if initialized.
   bool mInitialized;
   // True if this is the first command line.
   bool mFirstCmd;
   // Number of ints allocated in mValues.
   int  mAlloc;
   // Current index into mValues.
   int  mIndex;
   // True if all values were read in correctly.
   bool mValidFlag;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Intrastructure.

   // Constructor.
   CmdLineIntTable1D();
  ~CmdLineIntTable1D();
   void reset();

   // Initialize and allocate memory for the table.
   void initialize(int aRows);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Access components.
   int& e(int aRow);
   int& operator()(int aRow);
   void show(char* aLabel=0);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Execute, overload used to read from a command line file. This is called
   // for each line in the corresponding table section of the file. It parses 
   // the file command line to read table values
   void execute(Ris::CmdLineCmd* aCmd);

   // Execute again. reinitializes and calls execute.
   void reexecute(Ris::CmdLineCmd* aCmd);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Table, 2 dimensional, indexed by 0..R-1,0..C-1

class CmdLineIntTable2D : public Ris::BaseCmdLineExec
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Matrix components.
   int* mValues;
   int  mRows;
   int  mCols;

   // True if initialized.
   bool mInitialized;
   // True if this is the first command line.
   bool mFirstCmd;
   // Number of ints allocated in mValues.
   int  mAlloc;
   // Current index into mValues.
   int  mIndex;
   // True if all values were read in correctly.
   bool mValidFlag;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Intrastructure.

   // Constructor.
   CmdLineIntTable2D();
  ~CmdLineIntTable2D();
   void reset();

   // Initialize and allocate memory for the table.
   void initialize(int aRows,int aCols);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Access components.
   int& e(int aRow,int aCol);
   int& operator()(int aRow,int aCol);
   void show(char* aLabel=0);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Execute, overload used to read from a command line file. This is called
   // for each line in the corresponding table section of the file. It parses 
   // the file command line to read table values
   void execute(Ris::CmdLineCmd* aCmd);

   // Execute again. reinitializes and calls execute.
   void reexecute(Ris::CmdLineCmd* aCmd);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Table, 1 dimensional, indexed by 0..R-1

class CmdLineStringTable : public Ris::BaseCmdLineExec
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // String table components.
   char*  mStringStore;
   char** mStringPtr;
   int    mRows;
   int    mStringSize;

   // True if initialized.
   bool mInitialized;
   // True if this is the first command line.
   bool mFirstCmd;
   // Number of doubles allocated in mValues.
   int  mAlloc;
   // Current index into mValues.
   int  mIndex;
   // True if all values were read in correctly.
   bool mValidFlag;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   CmdLineStringTable();
  ~CmdLineStringTable();

   // Initialize and allocate memory for the table.
   void initialize(
      int     aRows,
      int     aStringSize);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Access components
   char* e(int aRow);
   char* operator()(int aRow);
   void show(char* aLabel=0);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Execute, overload used to read from a command line file. This is called
   // for each line in the corresponding table section of the file. It parses 
   // the file command line to read table values
   void execute(Ris::CmdLineCmd* aCmd);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

}//namespace




