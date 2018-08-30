//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risCmdLineExec.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

BaseCmdLineExec::BaseCmdLineExec()
{
   mExitFlag = false;
   mNestedLevel = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Exit.

void BaseCmdLineExec::forceExit()
{
   // This can be used to cause an "EXIT" command for the executer.
   mExitFlag=true;;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Push the executive onto the nested executive stack.

void BaseCmdLineExec::nestedPush(CmdLineCmd* aCmd,BaseCmdLineExec* aNextExec)
{
   // Guard.
   if (aCmd->mNestedAnchor == 0) return;

   // Push the executive onto the nested executive stack.
   aCmd->mNestedAnchor->nestedPush(aNextExec);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Pop the executive from the nested executive stack.

void BaseCmdLineExec::nestedPop(CmdLineCmd* aCmd)
{
   // Guard.
   if (aCmd->mNestedAnchor == 0) return;

   // Pop the executive from the nested executive stack.
   aCmd->mNestedAnchor->nestedPop();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Test the command for a brace change in the nested level.
// If it is the end of a nested level then pop back out.

void BaseCmdLineExec::testNestedLevel(CmdLineCmd* aCmd)
{
   // If it is the beginning of a nested level then increment the count.
   if(aCmd->isCmd("{"    ))  mNestedLevel++;

   // If it is the end of a nested level then decrement the count.
   if (aCmd->isCmd("}"))
   {
      if (--mNestedLevel == 0)
      {
         // Pop back out at the end.
         nestedPop(aCmd);
      }
   }

}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

CmdLineExecNestedAnchor::CmdLineExecNestedAnchor()
{
   mChangeFlag = false;
   mExec = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize.

void CmdLineExecNestedAnchor::initialize(BaseCmdLineExec* aInitialExec)
{
   mChangeFlag = false;
   mExec = aInitialExec;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Push the executive onto the nested executive stack.

void CmdLineExecNestedAnchor::nestedPush(BaseCmdLineExec* aNextExec)
{
   // Push current executive onto the stack.
   mExecStack.push(mExec);

   // Set the current executive.
   mExec = aNextExec;

   // Change.
   mChangeFlag = true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Pop the executive from the nested executive stack.

void CmdLineExecNestedAnchor::nestedPop  ()
{
   // Pop from the stack to set the current executive.
   mExecStack.pop(mExec);

   // Change.
   mChangeFlag = true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

