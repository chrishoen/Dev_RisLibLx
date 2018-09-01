#pragma once

/*==============================================================================
Command line executive base class.
==============================================================================*/

#include "risCmdLineCmd.h"
#include "risContainers.h"


//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Command Line Executive base class.
// This is a base class for classes that process command line strings, which
// contain a command and arguments. Pointers to the base class are passed to
// command line executers (instancesof CmdLineConsole or CmdLineFile) to process
// input from a console command line or input from a command line file, which is
// a text file that contains a sequence of command lines.

class CmdLineExecNestedAnchor;

class BaseCmdLineExec
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Nested brace level.
   int mNestedLevel;

   // If true then an exit has been entered or forced.
   bool   mExitFlag;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Intrastructure.

   // Constructor
   BaseCmdLineExec();
   virtual void reset (){};

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // This is overridden by inheriting classes to execute individual command
   // lines. This is called by command line processors that process sequences
   // of command lines.
   virtual void execute(CmdLineCmd* aCmd)=0;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Nested anchor, used for processing nested records. The command contains
   // a pointer to the nested anchor
   void nestedPush (CmdLineCmd* aCmd, BaseCmdLineExec* aNextExec);
   void nestedPop  (CmdLineCmd* aCmd);

   // Test the command for a brace change in the nested level.
   // If it is the end of a nested level then pop back out.
   void testNestedLevel (CmdLineCmd* aCmd);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // This can be called to indicate to the command line processor to exit a
   // sequence of commands, as if "EXIT" was the command.
   void forceExit();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Command line executive nested anchor.
// This is used for sequences of commands that process nested records, which are
// processed by different executives. It contains a stack of executive pointers.
// When processing for a nested record is entered, the current executive is
// pushed onto the stack. When the processing is exited, the executive is
// restored by popping it from the stack.

class CmdLineExecNestedAnchor
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // True if there was a push or pop.
   bool mChangeFlag;

   // Current executive.
   BaseCmdLineExec* mExec;

   // Stack of nested executive pointers.
   Ris::Containers::Stack<BaseCmdLineExec*,20> mExecStack;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Intrastructure.

   // Constructor.
   CmdLineExecNestedAnchor();
   // Initialize.
   void initialize(BaseCmdLineExec* aInitialExec);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Push executive onto the stack and set current executive.
   void nestedPush(BaseCmdLineExec* aNextExec);
   // Pop executive from the stack to set the current executive.
   void nestedPop();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

