/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "my_functions.h"
#include "my_defs.h"
#include "risCmdLineCmd.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Regional functions

// Convert a string to all upper case.
static void r_myStrupr(char* aString);

// Similar to strtok
static char* r_myStrtok(char* aString,char* aDelimiters,int* aIndexPtr);

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

CmdLineCmd::CmdLineCmd()
{
   reset();
}

void CmdLineCmd::reset()
{
   mGoodCmd = false;
   strcpy(mDelimiters, " \t");

   for (int i = 0; i < MaxNumOfArgs; i++)
   {
      mArgPtr[i] = &mArgStore[i][0];
   }

   mNestedAnchor = 0;
}

CmdLineCmd::CmdLineCmd(const char* aCommandLine)
{
   reset();
   parseCmdLine(aCommandLine);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Parse command line string into command and argument members.

void CmdLineCmd::parseCmdLine(const char* aCommandLine)
{
   //---------------------------------------------------------------------------
   // Locals

   int   i;
   char* tToken;
   int   tArgIndex=1;
   int   tTokenIndex=0;

   //---------------------------------------------------------------------------
   // Initialize members

   my_strncpy(mCommandLine,aCommandLine,1000);
   mGoodCmd=false;

   for (i=0;i<=MaxNumOfArgs;i++)
   {
      mArgFlag[i]=false;
   }

   mArgNum      = 0;
   mValidFlag   = 0;
   mArgWhole[0] = 0;

   //---------------------------------------------------------------------------
   // Guard

   if(strlen(mCommandLine)==0) return;

   //---------------------------------------------------------------------------
   // Parse command line into command and argument members

   tToken = r_myStrtok(mCommandLine,mDelimiters,&tTokenIndex);

   // Guard against line with one space
   if(tToken==0) return;

   // Set valid
   mValidFlag = 1;

   // Store command in arg[0]
   strcpy(mArgPtr[0],tToken);
   // Set arg[0] valid flag
   mArgFlag[0] = true;

   // Loop through command line to extract argument pointers
   // First argument is at arg[1]
   tArgIndex = 1;
   while((tToken = r_myStrtok(mCommandLine,mDelimiters,&tTokenIndex)))
   {
      // Store argument pointer
      strcpy(mArgPtr[tArgIndex],tToken);

      // Set argument valid flag 
      mArgFlag [tArgIndex]=true;

      // Test for max
      if(mArgNum==MaxNumOfArgs) break;

      // Advance
      mArgNum++;
      tArgIndex++;
   }

   // Copy all of the chars after the command into the whole argument.
   if (mArgNum == 0) return;

   int tCmdLength = (int)strlen(mArgPtr[0]);
   const char* tArgWholePtr = &aCommandLine[0] + tCmdLength + 1;
   my_strncpy(mArgWhole, tArgWholePtr, 200);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Compare arg[0] with a string.

bool CmdLineCmd::isCmd(const char* aString)
{
   // Guard 
   if (!mValidFlag) return false;

   // Copy to temp and convert to upper case
   char tArg0[80];
   my_strncpy(tArg0,mArgPtr[0],80);
   r_myStrupr(tArg0);

   // Copy to temp and convert to upper case
   char tString[80];
   my_strncpy(tString,aString,80);
   r_myStrupr(tString);

   // Compare mCommand and aString 
   if ((strcmp(tArg0,tString)==0))
   {
      // Set good command
      mGoodCmd=true;
      return true;
   }
   else
   {
      return false;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return true if no call to isCmd returned true.

bool CmdLineCmd::isBadCmd()
{
   return(!mGoodCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return number of arguments.

int CmdLineCmd::numArg()
{
   // Return number of arguments.
   return mArgNum;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return argument value.

int CmdLineCmd::argInt(int aArgIndex)
{
   int tValue=0;

   // Guard.
   if(!mArgFlag[aArgIndex]) return 0;

   // Convert argument string.
   sscanf(mArgPtr[aArgIndex],    "%d", &tValue);
   if (tValue==0)
   {
      sscanf(mArgPtr[aArgIndex], "%x", &tValue);
   }

   // Return argument value.
   return tValue;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return argument value.

unsigned int CmdLineCmd::argUnsigned(int aArgIndex)
{
   unsigned int tValue=0;

   // Guard.
   if(!mArgFlag[aArgIndex]) return 0;

   // Convert argument string.
   sscanf(mArgPtr[aArgIndex],    "%d", &tValue);
   if (tValue==0)
   {
      sscanf(mArgPtr[aArgIndex], "%x", &tValue);
   }

   // Return argument value.
   return tValue;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return argument value.

bool CmdLineCmd::argBool(int aArgIndex)
{
   // Guard.
   if(!mArgFlag[aArgIndex]) return false;

   // Test for 1,0

   // Convert argument string.
   if (strcmp(mArgPtr[aArgIndex],"1")==0)       return true;
   if (strcmp(mArgPtr[aArgIndex],"0")==0)       return false;

   // Test for true,false

   // Copy to temp and convert to upper case.
   char tArgN[80];
   my_strncpy(tArgN,mArgPtr[aArgIndex],80);
   r_myStrupr(tArgN);

   // Copy to temp and convert to upper case.
   char tString[80];
   strcpy(tString,"TRUE");

   // Compare aString with argument string.
   return (strcmp(tArgN,tString)==0);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return argument value.

double CmdLineCmd::argDouble(int aArgIndex)
{
   double tValue=0.0;

   // Guard.
   if(!mArgFlag[aArgIndex]) return 0;

   // Convert argument string.
   tValue = atof(mArgPtr[aArgIndex]);

   // Return argument value.
   return tValue;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return argument value.

double CmdLineCmd::argAngle(int aArgIndex)
{
   double tValue=0.0;

   // Guard.
   if(!mArgFlag[aArgIndex]) return 0;

   // Convert argument string.
   tValue = atof(mArgPtr[aArgIndex]);

   // Return argument value.
   // Convert from degrees to radians.
   return tValue*MY_RAD;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return argument value.

const char* rCmdEmptyString = "NULL";

char* CmdLineCmd::argString(int aArgIndex)
{
   char* tValue = (char*)rCmdEmptyString;

   // Guard.
   if(!mArgFlag[aArgIndex]) return (char*)rCmdEmptyString;

   // Convert argument string.
   tValue = mArgPtr[aArgIndex];

   // Return char* argument.
   return tValue;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return a pointer to the whole argumant string.

char* CmdLineCmd::argWhole()
{
   return &mArgWhole[0];
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Copy the whole argument into a string

void CmdLineCmd::copyArgWhole(char* aString, int aMaxSize)
{
   // Copy argument to string.
   my_strncpy(aString, mArgWhole, aMaxSize);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return argument value.

bool CmdLineCmd::isArgString (int aArgIndex,const char* aString)
{
   // Guard.
   if (!mValidFlag) return false;

   // Guard.
   if(!mArgFlag[aArgIndex]) return false;

   // Copy to temp and convert to upper case.
   char tArgN[80];
   my_strncpy(tArgN,mArgPtr[aArgIndex],80);
   r_myStrupr(tArgN);

   // Copy to temp and convert to upper case.
   char tString[80];
   my_strncpy(tString,aString,80);
   r_myStrupr(tString);

   // Compare aString with argument string.
   return (strcmp(tArgN,tString)==0);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return argument value.

void CmdLineCmd::copyArgString (int aArgIndex,char* aString,int aMaxSize)
{
   // Guard.
   if(!mArgFlag[aArgIndex])
   {
	   aString[0]=0;
	   return;
   }

   // Copy argument to string.
   my_strncpy(aString,mArgPtr[aArgIndex],aMaxSize);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return true if the argument is a number.

bool CmdLineCmd::isArgNumber(int aArgIndex)
{
   // Guard.
   if (!mArgFlag[aArgIndex]) return false;

   // Determine if the argument is an integer.
   char* p = 0;

   p = 0;
   strtol(mArgPtr[aArgIndex], &p, 0);
   if (p == 0) return false;
   if (*p == 0) return true;

   p = 0;
   strtod(mArgPtr[aArgIndex], &p);
   if (p == 0) return false;
   if (*p == 0) return true;

   return false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Set default argument.

void CmdLineCmd::setArgDefault(int aArgIndex, int aValue)
{
   if (aArgIndex <= mArgNum) return;

   sprintf(mArgPtr[aArgIndex],"%d",aValue);

   mArgFlag[aArgIndex] = true;
   mArgNum = aArgIndex;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Set default argument.

void CmdLineCmd::setArgDefaultUnsigned(int aArgIndex, unsigned aValue)
{
   if (aArgIndex <= mArgNum) return;

   sprintf(mArgPtr[aArgIndex],"%d",aValue);

   mArgFlag[aArgIndex] = true;
   mArgNum = aArgIndex;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Set default argument.

void CmdLineCmd::setArgDefault(int aArgIndex, bool aValue)
{
   if (aArgIndex <= mArgNum) return;

   strcpy(mArgPtr[aArgIndex],aValue?"TRUE":"FALSE");

   mArgFlag[aArgIndex] = true;
   mArgNum = aArgIndex;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Set default argument.

void CmdLineCmd::setArgDefault(int aArgIndex, double aValue)
{
   if (aArgIndex <= mArgNum) return;

   sprintf(mArgPtr[aArgIndex],"%f",aValue);

   mArgFlag[aArgIndex] = true;
   mArgNum = aArgIndex;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Set default argument.

void CmdLineCmd::setArgDefault(int aArgIndex, char* aValue)
{
   if (aArgIndex <= mArgNum) return;

   strcpy(mArgPtr[aArgIndex],aValue);

   mArgFlag[aArgIndex] = true;
   mArgNum = aArgIndex;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Convert string to upper case.

void r_myStrupr(char* aString)
{
   unsigned char* tPtr = (unsigned char*)aString;
   int tIndex=0;
   while (tPtr[tIndex]!=0)
   {
      int tValue = (int)tPtr[tIndex];
      if ((tValue >= 97)&&(tValue <= 122))
      {
         tValue -= 32;
         tPtr[tIndex] = (unsigned char)tValue;
      }
      tIndex++;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// String token analog.

char* r_myStrtok(char* aString,char* aDelimiters,int* aIndexPtr)
{
   //---------------------------------------------------------------------------
   // Locals

   char* tToken=0;

   int tIndex = *aIndexPtr;
   int tNumOfDelimiters = (int)strlen(aDelimiters);
   bool tGoing=true;

   //---------------------------------------------------------------------------
   // Search for first occurrance of a non delimiter

   tGoing=true;
   while(tGoing)
   {
      // If not end of string
      if (aString[tIndex]!=0)
      {
         // Compare current char with all delimiters
         bool tDelimiter = false;
         for(int i=0;i<tNumOfDelimiters;i++)
         {
            if (aString[tIndex]==aDelimiters[i]) 
            {
               tDelimiter=true;
            }
         }
         // If current char is not a delimiter 
         if (tDelimiter==false)
         {
            // Store token pointer and exit loop
            tToken = &aString[tIndex];
            tGoing=false;
         }
         // If current char is a delimiter 
         else
         {
            // Advance to next char
            tIndex++;
         }
      }
      // Else end of string
      else
      {
         // Exit loop
         tGoing=false;
      }
	}

   //---------------------------------------------------------------------------
   // If no token then update state and return 0

   if(tToken==0)
   {
      *aIndexPtr=tIndex;
      return 0;
   }

   //---------------------------------------------------------------------------
   // If the first char in the token is not a quote

   if(tToken[0]!='\"')
   {
      //------------------------------------------------------------------------
      // Search for next occurrance of a delimiter

      tGoing=true;
      while(tGoing)
      {
         // If not end of string
         if (aString[tIndex]!=0)
         {
            // Compare current char with all delimiters
            for(int i=0;i<tNumOfDelimiters;i++)
            {
               // If current char is a delimiter 
               if (aString[tIndex]==aDelimiters[i]) 
               {
                  // Replace current char with end of string
                  // This terminates the token
                  aString[tIndex]=0;
                  // Exit the loop
                  tGoing=false;
               }
            }
            // Advance to the next char
            tIndex++;
         }
         // Else end of string
         else
         {
            // Exit the loop
            tGoing=false;
         }
	   }
   }
   //---------------------------------------------------------------------------
   // Else the first char is a quote
   else
   {
      // Bypass the quote
      tIndex++;
      tToken++;
      //------------------------------------------------------------------------
      // Search for next occurrance of quote
      tGoing=true;
      while(tGoing)
      {
         if (aString[tIndex]!=0)
         {
            if (aString[tIndex]=='\"') 
            {
               aString[tIndex]=0;
               tGoing=false;
            }
            tIndex++;
         }
         else
         {
            tGoing=false;
         }
	   }
   }

   //------------------------------------------------------------------------
   // Update state and return token

   *aIndexPtr=tIndex;
   return tToken;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

