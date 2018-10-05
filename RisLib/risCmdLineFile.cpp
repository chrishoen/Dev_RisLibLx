/*==============================================================================
==============================================================================*/

#include "stdafx.h"

#include "my_functions.h"
#include "risContainers.h"

#include "risCmdLineFile.h"


namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Open the file.

bool CmdLineFile::open(char* aFilename)
{            
   mFile = fopen(aFilename,"r");

   if (mFile==0) return false;

   mLineNumber=0;

   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Close the file.

void CmdLineFile::close()
{       
   // Close the file
   if (mFile) fclose(mFile);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Loop to read each line of the file. Process each line with the given command
// line executive.

void CmdLineFile::execute(BaseCmdLineExec* aExec)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Do this first.

   // Guard.
   if (mFile==0) return;

   // Command line command.
   CmdLineCmd tCmd;

   // Command line executive pointer used for nested executives.
   BaseCmdLineExec* tExec = aExec;

   // Command line variables.
   char tCommandLine[200];
   int  tCommandLineLen=0;
   bool tCommentFlag=false;

   // Initialize the nested anchor, it is used for nested sections in the
   // file, nested sections execute different executives.
   mNestedAnchor.initialize(aExec);

   // Set the command nested anchor pointer. This is piggybacked onto the 
   // command so that it can be used by the executive.
   tCmd.mNestedAnchor = &mNestedAnchor;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Loop to read each command line in the file. If it contains a valid
   // command then pass it to the current executive.
            
   bool tGoing=true;
   while(tGoing)
   {
      // Read command line from the file.
      if (fgets(tCommandLine,200,mFile)==0) break;
      mLineNumber++;

      // Test for comment line.
      tCommentFlag=false;
      if (tCommandLine[0]=='#') tCommentFlag=true;
      if (tCommandLine[0]=='/') tCommentFlag=true;

      // Remove cr/lf at end of line.
      my_trimCRLF(tCommandLine);

      // Command line length.
      tCommandLineLen=(int)strlen(tCommandLine);

      // If the command line is not empty and not a comment then process
      // it, else continue the loop and go on to the next line.
      if(tCommandLineLen>2 && !tCommentFlag)
      {
         // Parse the command line into the command object.
         tCmd.parseCmdLine(tCommandLine);
   
         // If exit command then exit the loop.
         if(tCmd.isCmd("EXIT"))
         {
            tGoing=false; 
         }
         // If not an exit then process the parsed command line.
         else
         {
            // If tExec is not zero.
            if (tExec != 0)
            {
               // Call the executive to process the command.
               tExec->execute(&tCmd);
            }
            // Else tExec is not valid, there was a prior unrecognized Begin 
            // and zero was nestedPushed.
            else
            {
               if (tCmd.isCmd("END"))
               {
                  // Pop the executive from the nested executive stack
                  // This will balance the unrecognized Begin.
                  mNestedAnchor.nestedPop();
               }
            }

            // If the command pushed to the anchor stack.
            if (mNestedAnchor.mChangeFlag)
            {
                // If the command pushed a new executive onto the anchor
                // stack because it entered a new nested section, or if it 
                // popped an executive from the anchor stack because it is
                // leaving a nested section then get the next executive from
                // the anchor stack.
                mNestedAnchor.mChangeFlag = false;
                tExec = mNestedAnchor.mExec;
            }

            // If the command set the exit flag then exit the loop.
            if (tExec)
            {
               if (tExec->mExitFlag)
               {
                  tGoing = false;
               }
            }
         }
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

