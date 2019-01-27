/*==============================================================================
Print utility
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risAlphaDir.h"

namespace Ris
{

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Regional variables

char rAlphaDirPath    [cMaxAlphaStringSize];

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Set the alpha directory file path. All subsequent calls will be relative
// to this.

void setAlphaDirectory(char* aDirPath)
{
   strcpy(rAlphaDirPath, aDirPath);
}

// Return the alpha directory path. The input is a temp char buffer.
// The buffer is written to with the requested string to and a pointer
// to it is returned.

char* getAlphaDirectory()
{
   return &rAlphaDirPath[0];
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Return a file path to a file that lives in alpha. The first input is a
// temp char buffer. The second input is an file name. If the file name is
// null then the dir path is returned. The buffer is written to with the 
// requested string to and a pointer to it is returned.

char* getAlphaFilePath_X(char* aBuffer, char* aSubDirName, char* aFileName = 0)
{
   strcpy(aBuffer, rAlphaDirPath);
   strcat(aBuffer, aSubDirName);
   strcat(aBuffer, "/");

   if (aFileName == 0) return aBuffer;

   strcat(aBuffer, aFileName);
   return aBuffer;
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Return a file path to a file that lives in alpha. The first input is a
// temp char buffer. The second input is an file name. If the file name is
// null then the dir path is returned. The buffer is written to with the 
// requested string to and a pointer to it is returned.

char* getAlphaFilePath_Bin         (char* aBuffer, char* aFileName) { return getAlphaFilePath_X(aBuffer, "bin",         aFileName); }
char* getAlphaFilePath_Calibration (char* aBuffer, char* aFileName) { return getAlphaFilePath_X(aBuffer, "calibration", aFileName); }
char* getAlphaFilePath_Data        (char* aBuffer, char* aFileName) { return getAlphaFilePath_X(aBuffer, "data",        aFileName); }
char* getAlphaFilePath_DLL         (char* aBuffer, char* aFileName) { return getAlphaFilePath_X(aBuffer, "dll",         aFileName); }
char* getAlphaFilePath_Image       (char* aBuffer, char* aFileName) { return getAlphaFilePath_X(aBuffer, "image",       aFileName); }
char* getAlphaFilePath_Lib         (char* aBuffer, char* aFileName) { return getAlphaFilePath_X(aBuffer, "lib",         aFileName); }
char* getAlphaFilePath_Log         (char* aBuffer, char* aFileName) { return getAlphaFilePath_X(aBuffer, "log",         aFileName); }
char* getAlphaFilePath_RecData     (char* aBuffer, char* aFileName) { return getAlphaFilePath_X(aBuffer, "recdata",     aFileName); }
char* getAlphaFilePath_Settings    (char* aBuffer, char* aFileName) { return getAlphaFilePath_X(aBuffer, "settings",    aFileName); }
char* getAlphaFilePath_Work        (char* aBuffer, char* aFileName) { return getAlphaFilePath_X(aBuffer, "work",        aFileName); }

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize the regional variables when program is loaded

class AlphaResetClass
{
public:
   AlphaResetClass()
   {
      strcpy(rAlphaDirPath, "/opt/alpha/");
   }
};

AlphaResetClass gAlphaResetClass;

//******************************************************************************
//******************************************************************************
//******************************************************************************
} //namespace

