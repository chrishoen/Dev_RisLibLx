//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risAlphaDir.h"
#include "risPortableCalls.h"
#include "risCmdLineFile.h"

#define  _RISCMDLINEPARMS_CPP_
#include "risCmdLineParms.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

BaseCmdLineParms::BaseCmdLineParms()
{
   mExplicitFileDir[0]=0;
   mUseExplicitFileDir=false;
   reset();
}

void BaseCmdLineParms::reset()
{
   mUseSections = true;
   mEnablePrint = true;
   mTargetSection[0] = 0;
   mTargetSectionFlag = true;
   strcpy(mName, "NO_NAME");
   strcpy(mDefaultFileName, "NO_DEFAULT_FILENAME");
   strcpy(mOverrideFileName,"NO_OVERRIDE_FILENAME");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return true if the command is in the target section.

bool BaseCmdLineParms::isTargetSection(Ris::CmdLineCmd* aCmd)
{
   // If not using sections then always in target section.
   if (!mUseSections) return true;

   // If the command is the beginning of a section. 
   if (aCmd->isCmd("SectionBegin"))
   {
      mTargetSectionFlag = false;

      if (aCmd->numArg() == 1)
      {
         // And the section is the target section then the 
         // command is in the target section.
         if (aCmd->isArgString(1, mTargetSection))
         {
            mTargetSectionFlag = true;
         }
      }
   }

   // If the command is at the end of any section
   if (aCmd->isCmd("SectionEnd"))
   {
      // Then the command is not in the target section.
      mTargetSectionFlag = false;
   }

   return mTargetSectionFlag;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Read a section of the command file and set member variables accordingly.
// Create a command file object, open the file, pass this object to the file
// object to read the file and apply this object's execution method to each
// command in the file, and then close the file. This only reads variables
// for a specific section in the file. If the input section is null or 
// empty then section logic is ignored and the entire file is read.

bool BaseCmdLineParms::readSection(char* aFilePath, char* aSection)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Section variables.   

   // If the input section is invalid then don't use sections.
   if (aSection == 0)
   {
      mUseSections = false;
   }
   else if (strlen(aSection) == 0)
   {
      mUseSections = false;
   }

   // Store arguments.
   if (mUseSections)
   {
      strcpy(mTargetSection, aSection);
   }
   else
   {
      strcpy(mTargetSection, "NO_SECTIONS");
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Open file.

   // Command line executive file object.   
   Ris::CmdLineFile tCmdLineFile;

   if (!tCmdLineFile.open(aFilePath))
   {
      if(mEnablePrint) printf("BaseCmdLineParms::file open FAIL001 %s %s\n", mDefaultFileName,aFilePath);
      return false;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Execute the commands in the file.

   // Pass this object to the file object to read the file and apply this
   // object's execution method to each command in the file.
   tCmdLineFile.execute(this);

   // Close the file.
   tCmdLineFile.close();

   // Expand extra member variables.
   expand();

   // Done.
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is the same as above, but with a null filepath. The filepath is
// searched amoung default filepaths.

bool BaseCmdLineParms::readSection(char* aSection)
{
   char tFilePath[400];

   // Filepath found.
   bool tFileFound = false;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Search for file. If found then read it.

   if (mUseExplicitFileDir)
   {
      strcpy(tFilePath, mExplicitFileDir);
      strcat(tFilePath, mDefaultFileName);

      if (portableFilePathExists(tFilePath))
      {
         return readSection(tFilePath, aSection);
      }
      else
      {
         if (mEnablePrint) printf("BaseCmdLineParms::file open FAIL101 %s %s\n", mDefaultFileName, tFilePath);
         return false;
      }
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Search for file. If found then read it.

   strcpy(tFilePath, Ris::portableGetProgramDir());
   strcat(tFilePath, mDefaultFileName);

   if (portableFilePathExists(tFilePath))
   {
      return readSection(tFilePath,aSection);
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Search for file. If found then read it.

   strcpy(tFilePath, Ris::portableGetCurrentDir());
   strcat(tFilePath, mDefaultFileName);

   if (portableFilePathExists(tFilePath))
   {
      return readSection(tFilePath,aSection);
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Search for file. If found then read it.

   strcpy(tFilePath, Ris::portableGetCurrentDir());
   strcat(tFilePath, "..\\Files\\");
   strcat(tFilePath, mDefaultFileName);

   if (portableFilePathExists(tFilePath))
   {
      return readSection(tFilePath,aSection);
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Search for file. If found then read it.

   strcpy(tFilePath, Ris::portableGetProgramDir());
   strcat(tFilePath, "..\\..\\Files\\");
   strcat(tFilePath, mDefaultFileName);

   if (portableFilePathExists(tFilePath))
   {
      return readSection(tFilePath,aSection);
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Search for file. If found then read it.

   char tBuffer[400];
   strcpy(tFilePath, Ris::getAlphaFilePath_Work(tBuffer, mDefaultFileName));

   if (portableFilePathExists(tFilePath))
   {
      return readSection(tFilePath,aSection);
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Search for file. If found then read it.

   strcpy(tFilePath, Ris::getAlphaFilePath_Settings(tBuffer, mDefaultFileName));

   if (portableFilePathExists(tFilePath))
   {
      return readSection(tFilePath, aSection);
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // File not found.

   if (mEnablePrint) printf("BaseCmdLineParms::file open FAIL102 %s %s\n", mDefaultFileName,tFilePath);
   return false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is the same as above, but it uses the override file name, instead
// of the default file name.

bool BaseCmdLineParms::readOverride(char* aSection)
{
   char tFilePath[200];

   // Filepath found.
   bool tFileFound = false;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Search for file. If found then read it.

   if (mUseExplicitFileDir)
   {
      strcpy(tFilePath, mExplicitFileDir);
      strcat(tFilePath, mOverrideFileName);

      if (portableFilePathExists(tFilePath))
      {
         return readSection(tFilePath, aSection);
      }
      else
      {
         if (mEnablePrint) printf("BaseCmdLineParms::file open FAIL201 %s %s\n", mDefaultFileName, tFilePath);
         return false;
      }
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Search for file. If found then read it.

   strcpy(tFilePath, Ris::portableGetCurrentDir());
   strcat(tFilePath, mOverrideFileName);

   if (portableFilePathExists(tFilePath))
   {
      return readSection(tFilePath,aSection);
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Search for file. If found then read it.

   strcpy(tFilePath, Ris::portableGetCurrentDir());
   strcat(tFilePath, "..\\Files\\");
   strcat(tFilePath, mOverrideFileName);

   if (portableFilePathExists(tFilePath))
   {
      return readSection(tFilePath,aSection);
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Search for file. If found then read it.

   strcpy(tFilePath, Ris::portableGetProgramDir());
   strcat(tFilePath, "..\\..\\Files\\");
   strcat(tFilePath, mOverrideFileName);

   if (portableFilePathExists(tFilePath))
   {
      return readSection(tFilePath,aSection);
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Search for file. If found then read it.

   char tBuffer[400];
   strcpy(tFilePath, Ris::getAlphaFilePath_Work(tBuffer, mOverrideFileName));

   if (portableFilePathExists(tFilePath))
   {
      return readSection(tFilePath,aSection);
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // File not found.

   if (mEnablePrint) printf("BaseCmdLineParms::file open FAIL202 %s %s\n", mDefaultFileName,tFilePath);
   return false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Set the file dir that the parms file lives in.
// The following read functions search for the parms file in the 
// functions search first the current working directory and then the 
// current program directory.
// 1) the explicit file dir, if it is set
// 2) the current working directory
// 3) the current working directory + ..\Files
// 4) the current program directory

void BaseCmdLineParms::setExplicitFileDir(char* aFileDir)
{
   strcpy(mExplicitFileDir, aFileDir);
   mUseExplicitFileDir = true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Set the default file name.
// Set the override file name.

void BaseCmdLineParms::setDefaultFileName(char* aFileName)
{
   strcpy(mDefaultFileName, aFileName);
}
void BaseCmdLineParms::setOVerrideFileName(char* aFileName)
{
   strcpy(mOverrideFileName, aFileName);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Alias for readSection(0);

bool BaseCmdLineParms::readFile()
{
   return readSection(0);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace


