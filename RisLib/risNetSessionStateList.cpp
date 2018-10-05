/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "prnPrint.h"

#include "risNetSessionStateList.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

SessionStateList::SessionStateList()
{
   mNumSessions=0;
   for (int index=0;index<cMaxSessions;index++)
   {
      mIdent[index] = cInvalidValue;
      mPeer[index] = cInvalidValue;
      mStateCode[index]= cInvalidValue;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

// Add an ordered pair to the list
void SessionStateList::add(int aIndex,int aIdent)
{
   mIdent[aIndex]=aIdent;
   mNumSessions++;
}

// Remove an ordered pair from the list
void SessionStateList::remove(int aIndex)
{
   mIdent[aIndex] = cInvalidValue;
   mNumSessions--;
}

// Get the identifier associated with a session index
int SessionStateList::getIdent(int aIndex)
{
   return mIdent[aIndex];
}

// Get the session index associated with an identifier
int SessionStateList::getIndex(int aIdent)
{
   for (int index=0;index<cMaxSessions;index++)
   {
      if (mIdent[index]==aIdent)
      {
         return index;
      }
   }
   return cInvalidValue; 
}

// Get any identifier, used when clients request a zero ident 
int SessionStateList::getAnyIdent()
{
   for (int index=0;index<cMaxSessions;index++)
   {
      if (mIdent[index]!=cInvalidValue)
      {
         return mIdent[index];
      }
   }
   return cInvalidValue; 
}

// Is session index valid ?
bool SessionStateList::isValidIndex(int aIndex)
{
   if (aIndex<0) return false;
   return (mIdent[aIndex]!=cInvalidValue);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void SessionStateList::show(char* aLabel)
{
   Prn::print(0,"############################# %s BEGIN",aLabel);

   Prn::print(0,"mNumSessions %d",mNumSessions);

   for (int index=0;index<cMaxSessions;index++)
   {
      int ident = mIdent[index];
      if (ident!=cInvalidValue)
      {
         Prn::print(0,"index,ident %d %d",index,ident);
      }
   }

   Prn::print(0,"############################# %s END",aLabel);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace
