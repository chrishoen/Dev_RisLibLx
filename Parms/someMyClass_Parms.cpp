/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "someMyClass.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor

MyClassParms::MyClassParms()
{
   reset();
}

void MyClassParms::reset()
{
   mEX1 = 0.0;
   mEX2 = 0.0;
   mEX3 = 0.0;
   mEX4 = 0.0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Execute a command line in the section of the parms file that is specific
// to this object and set member variables accordingly. When an "End" is
// encountered, pop back out of the section and return control to its parent
// executive.

void MyClassParms::execute(Ris::CmdLineCmd* aCmd)
{
   // Execute commands to read parameter members.
   if(aCmd->isCmd("EX1"               )) mEX1  = aCmd->argDouble(1);
   if(aCmd->isCmd("EX2"               )) mEX2  = aCmd->argDouble(1);
   if(aCmd->isCmd("EX3"               )) mEX3  = aCmd->argDouble(1);
   if(aCmd->isCmd("EX4"               )) mEX4  = aCmd->argDouble(1);

   // Pop back out at the end.
   if(aCmd->isCmd("End"  ))  nestedPop(aCmd);
   if(aCmd->isCmd("}"    ))  nestedPop(aCmd);
}

void MyClassParms::show(char* aLabel)
{
   printf("\n");
   printf("%-23s BEGIN\n", aLabel);

   printf("mEX1               %10.4f\n",mEX1);
   printf("mEX2               %10.4f\n",mEX2);
   printf("mEX3               %10.4f\n",mEX3);
   printf("mEX4               %10.4f\n",mEX4);

   printf("%-25s END\n", aLabel);
   printf("\n");
}
   
}//namespace