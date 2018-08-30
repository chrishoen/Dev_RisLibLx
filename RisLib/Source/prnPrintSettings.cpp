//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"


#define  _PRNPRINTSETTINGS_CPP_
#include "prnPrintSettings.h"

namespace Prn
{


//******************************************************************************
//******************************************************************************
//******************************************************************************

PrintSettings::PrintSettings()
{
   reset();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize variables to defaults

void PrintSettings::reset()
{
   mInitFlag=false;
   mSuppressFlag=false;
   mSpecialFlag=false;
   mLogFileEnable=false;
   mAppNumber=0;;

   // All disabled, except entry zero
   for(int i=0;i<cFilterTableSize;i++)
   {
      mFilterTable  [i] = false;
      mConsoleTable [i] = 0;
   }
   mFilterTable[0] = true;

   mSectionFlag=true;
   mSection[0]=0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// For a given command line "Begin Section", this returns true
// if "Section", the first command line argument, is the same as the section 
// specified in initialize.

bool PrintSettings::isMySection(Ris::CmdLineCmd* aCmd)
{
   bool tFlag=false;

   if (aCmd->numArg()==1)
   {
      if (aCmd->isArgString(1,mSection))
      {
         tFlag=true;
      }
   }

   return tFlag;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// set filters

void PrintSettings::setFilter(int aFilter, bool aEnablePrint, int aConsole)
{
   mFilterTable  [aFilter] = aEnablePrint;
   mConsoleTable [aFilter] = aConsole;
}   	

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Try filters and topics

void PrintSettings::tryFilterStart()
{
   mFilterTry = -1;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void PrintSettings::tryFilterString (Ris::CmdLineCmd* aCmd,char* aFilterString,int aFilter)
{
   if (aCmd->isArgString(1,aFilterString))
   {
      mFilterTry=aFilter;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void PrintSettings::tryFilterSet(Ris::CmdLineCmd* aCmd)
{
   if (mFilterTry<0)    return;
   
   if (aCmd->numArg() == 2)
   {
      setFilter(mFilterTry, aCmd->argBool(2));
   }
   else
   {
      setFilter(mFilterTry, aCmd->argBool(2), aCmd->argInt(3));
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is called for each command line in the settings file.
// It processes commands, with arguments
// BEGIN starts a section, END exits a section
// Only commands for a section are processed

void PrintSettings::execute(Ris::CmdLineCmd* aCmd)
{
   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   // Section commands


	if(aCmd->isCmd("SectionBegin"      ))  mSectionFlag=isMySection(aCmd);
   if(aCmd->isCmd("SectionEnd"        ))  mSectionFlag=false;

   if (!mSectionFlag) return;

   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   // Only process commands for the section specified in initialize.

   //---------------------------------------------------------------------------
   if(aCmd->isCmd("TEST"       ))
   {
      printf("TEST1   %d\n",aCmd->argInt(1));
      printf("TEST2   %d\n",aCmd->argInt(2));
      printf("TEST3   %d\n",aCmd->argInt(3));
      printf("TEST4   %d\n",aCmd->argInt(4));
   }

   if(aCmd->isCmd("RESET"      ))  reset();

   //---------------------------------------------------------------------------
   // Detect a filter and store
   // For command "Filter String Enable"

   if(aCmd->isCmd("FILTER")) 
   {
      tryFilterStart();

      tryFilterString (aCmd, "PrintInit1",   PrintInit1   );
      tryFilterString (aCmd, "PrintInit2",   PrintInit2   );
      tryFilterString (aCmd, "PrintInit3",   PrintInit3   );
      tryFilterString (aCmd, "PrintInit4",   PrintInit4   );
      tryFilterString (aCmd, "PrintRun1",    PrintRun1    );
      tryFilterString (aCmd, "PrintRun2",    PrintRun2    );
      tryFilterString (aCmd, "PrintRun3",    PrintRun3    );
      tryFilterString (aCmd, "PrintRun4",    PrintRun4    );
      tryFilterString (aCmd, "SocketInit1",  SocketInit1  );
      tryFilterString (aCmd, "SocketInit2",  SocketInit2  );
      tryFilterString (aCmd, "SocketInit3",  SocketInit3  );
      tryFilterString (aCmd, "SocketInit4",  SocketInit4  );
      tryFilterString (aCmd, "SocketRun1",   SocketRun1   );
      tryFilterString (aCmd, "SocketRun2",   SocketRun2   );
      tryFilterString (aCmd, "SocketRun3",   SocketRun3   );
      tryFilterString (aCmd, "SocketRun4",   SocketRun4   );
      tryFilterString (aCmd, "SerialInit1",  SerialInit1  );
      tryFilterString (aCmd, "SerialInit2",  SerialInit2  );
      tryFilterString (aCmd, "SerialInit3",  SerialInit3  );
      tryFilterString (aCmd, "SerialInit4",  SerialInit4  );
      tryFilterString (aCmd, "SerialRun1",   SerialRun1   );
      tryFilterString (aCmd, "SerialRun2",   SerialRun2   );
      tryFilterString (aCmd, "SerialRun3",   SerialRun3   );
      tryFilterString (aCmd, "SerialRun4",   SerialRun4   );
      tryFilterString (aCmd, "FileInit1",    FileInit1    );
      tryFilterString (aCmd, "FileInit2",    FileInit2    );
      tryFilterString (aCmd, "FileInit3",    FileInit3    );
      tryFilterString (aCmd, "FileInit4",    FileInit4    );
      tryFilterString (aCmd, "FileRun1",     FileRun1     );
      tryFilterString (aCmd, "FileRun2",     FileRun2     );
      tryFilterString (aCmd, "FileRun3",     FileRun3     );
      tryFilterString (aCmd, "FileRun4",     FileRun4     );
      tryFilterString (aCmd, "ThreadInit1",  ThreadInit1  );
      tryFilterString (aCmd, "ThreadInit2",  ThreadInit2  );
      tryFilterString (aCmd, "ThreadInit3",  ThreadInit3  );
      tryFilterString (aCmd, "ThreadInit4",  ThreadInit4  );
      tryFilterString (aCmd, "ThreadRun1",   ThreadRun1   );
      tryFilterString (aCmd, "ThreadRun2",   ThreadRun2   );
      tryFilterString (aCmd, "ThreadRun3",   ThreadRun3   );
      tryFilterString (aCmd, "ThreadRun4",   ThreadRun4   );
      tryFilterString (aCmd, "ProcInit1",    ProcInit1    );
      tryFilterString (aCmd, "ProcInit2",    ProcInit2    );
      tryFilterString (aCmd, "ProcInit3",    ProcInit3    );
      tryFilterString (aCmd, "ProcInit4",    ProcInit4    );
      tryFilterString (aCmd, "ProcRun1",     ProcRun1     );
      tryFilterString (aCmd, "ProcRun2",     ProcRun2     );
      tryFilterString (aCmd, "ProcRun3",     ProcRun3     );
      tryFilterString (aCmd, "ProcRun4",     ProcRun4     );
      tryFilterString (aCmd, "PointInit1",   PointInit1   );
      tryFilterString (aCmd, "PointInit2",   PointInit2   );
      tryFilterString (aCmd, "PointInit3",   PointInit3   );
      tryFilterString (aCmd, "PointInit4",   PointInit4   );
      tryFilterString (aCmd, "PointRun1",    PointRun1    );
      tryFilterString (aCmd, "PointRun2",    PointRun2    );
      tryFilterString (aCmd, "PointRun3",    PointRun3    );
      tryFilterString (aCmd, "PointRun4",    PointRun4    );
      tryFilterString (aCmd, "QCallInit1",   QCallInit1   );
      tryFilterString (aCmd, "QCallInit2",   QCallInit2   );
      tryFilterString (aCmd, "QCallInit3",   QCallInit3   );
      tryFilterString (aCmd, "QCallInit4",   QCallInit4   );
      tryFilterString (aCmd, "QCallRun1",    QCallRun1    );
      tryFilterString (aCmd, "QCallRun2",    QCallRun2    );
      tryFilterString (aCmd, "QCallRun3",    QCallRun3    );
      tryFilterString (aCmd, "QCallRun4",    QCallRun4    );
      tryFilterString (aCmd, "CalcInit1",    CalcInit1    );
      tryFilterString (aCmd, "CalcInit2",    CalcInit2    );
      tryFilterString (aCmd, "CalcInit3",    CalcInit3    );
      tryFilterString (aCmd, "CalcInit4",    CalcInit4    );
      tryFilterString (aCmd, "CalcRun1",     CalcRun1     );
      tryFilterString (aCmd, "CalcRun2",     CalcRun2     );
      tryFilterString (aCmd, "CalcRun3",     CalcRun3     );
      tryFilterString (aCmd, "CalcRun4",     CalcRun4     );
      tryFilterString (aCmd, "CalcError1",   CalcError1   );
      tryFilterString (aCmd, "CalcError2",   CalcError2   );
      tryFilterString (aCmd, "CalcError3",   CalcError3   );
      tryFilterString (aCmd, "CalcError4",   CalcError4   );
      tryFilterString (aCmd, "MsgRx1",       MsgRx1       );
      tryFilterString (aCmd, "MsgRx2",       MsgRx2       );
      tryFilterString (aCmd, "MsgRx3",       MsgRx3       );
      tryFilterString (aCmd, "MsgRx4",       MsgRx4       );
      tryFilterString (aCmd, "ViewInit1",    ViewInit1    );
      tryFilterString (aCmd, "ViewInit2",    ViewInit2    );
      tryFilterString (aCmd, "ViewInit3",    ViewInit3    );
      tryFilterString (aCmd, "ViewInit4",    ViewInit4    );
      tryFilterString (aCmd, "ViewRun1",     ViewRun1     );
      tryFilterString (aCmd, "ViewRun2",     ViewRun2     );
      tryFilterString (aCmd, "ViewRun3",     ViewRun3     );
      tryFilterString (aCmd, "ViewRun4",     ViewRun4     );
      tryFilterString (aCmd, "CamInit1",     CamInit1     );
      tryFilterString (aCmd, "CamInit2",     CamInit2     );
      tryFilterString (aCmd, "CamInit3",     CamInit3     );
      tryFilterString (aCmd, "CamInit4",     CamInit4     );
      tryFilterString (aCmd, "CamRun1",      CamRun1      );
      tryFilterString (aCmd, "CamRun2",      CamRun2      );
      tryFilterString (aCmd, "CamRun3",      CamRun3      );
      tryFilterString (aCmd, "CamRun4",      CamRun4      );
      tryFilterString (aCmd, "CamError1",    CamError1    );
      tryFilterString (aCmd, "CamError2",    CamError2    );
      tryFilterString (aCmd, "CamError3",    CamError3    );
      tryFilterString (aCmd, "CamError4",    CamError4    );
      tryFilterString (aCmd, "SensorInit1",  SensorInit1  );
      tryFilterString (aCmd, "SensorInit2",  SensorInit2  );
      tryFilterString (aCmd, "SensorInit3",  SensorInit3  );
      tryFilterString (aCmd, "SensorInit4",  SensorInit4  );
      tryFilterString (aCmd, "SensorRun1",   SensorRun1   );
      tryFilterString (aCmd, "SensorRun2",   SensorRun2   );
      tryFilterString (aCmd, "SensorRun3",   SensorRun3   );
      tryFilterString (aCmd, "SensorRun4",   SensorRun4   );
      tryFilterString (aCmd, "SensorError1", SensorError1 );
      tryFilterString (aCmd, "SensorError2", SensorError2 );
      tryFilterString (aCmd, "SensorError3", SensorError3 );
      tryFilterString (aCmd, "SensorError4", SensorError4 );
      tryFilterString (aCmd, "DspInit1",     DspInit1     );
      tryFilterString (aCmd, "DspInit2",     DspInit2     );
      tryFilterString (aCmd, "DspInit3",     DspInit3     );
      tryFilterString (aCmd, "DspInit4",     DspInit4     );
      tryFilterString (aCmd, "DspRun1",      DspRun1      );
      tryFilterString (aCmd, "DspRun2",      DspRun2      );
      tryFilterString (aCmd, "DspRun3",      DspRun3      );
      tryFilterString (aCmd, "DspRun4",      DspRun4      );
      tryFilterString (aCmd, "DspError1",    DspError1    );
      tryFilterString (aCmd, "DspError2",    DspError2    );
      tryFilterString (aCmd, "DspError3",    DspError3    );
      tryFilterString (aCmd, "DspError4",    DspError4    );

      tryFilterString (aCmd, "Result1",      Result1      );
      tryFilterString (aCmd, "Result2",      Result2      );
      tryFilterString (aCmd, "Result3",      Result3      );
      tryFilterString (aCmd, "Result4",      Result4      );
      tryFilterString (aCmd, "Result5",      Result5      );
      tryFilterString (aCmd, "Result6",      Result6      );
      tryFilterString (aCmd, "Result7",      Result7      );
      tryFilterString (aCmd, "Result8",      Result8      );

      tryFilterString (aCmd, "Cal1",         Cal1         );
      tryFilterString (aCmd, "Cal2",         Cal2         );
      tryFilterString (aCmd, "Cal3",         Cal3         );
      tryFilterString (aCmd, "Cal4",         Cal4         );
      tryFilterString (aCmd, "Cal5",         Cal5         );
      tryFilterString (aCmd, "Cal6",         Cal6         );
      tryFilterString (aCmd, "Cal7",         Cal7         );
      tryFilterString (aCmd, "Cal8",         Cal8         );
      tryFilterString (aCmd, "CalError1",    CalError1    );
      tryFilterString (aCmd, "CalError2",    CalError2    );
      tryFilterString (aCmd, "CalError3",    CalError3    );
      tryFilterString (aCmd, "CalError4",    CalError4    );

      tryFilterString (aCmd, "BackInit1",    BackInit1    );
      tryFilterString (aCmd, "BackInit2",    BackInit2    );
      tryFilterString (aCmd, "BackInit3",    BackInit3    );
      tryFilterString (aCmd, "BackInit4",    BackInit4    );
      tryFilterString (aCmd, "BackRun1",     BackRun1     );
      tryFilterString (aCmd, "BackRun2",     BackRun2     );
      tryFilterString (aCmd, "BackRun3",     BackRun3     );
      tryFilterString (aCmd, "BackRun4",     BackRun4     );
      tryFilterString (aCmd, "BackError1",   BackError1   );
      tryFilterString (aCmd, "BackError2",   BackError2   );
      tryFilterString (aCmd, "BackError3",   BackError3   );
      tryFilterString (aCmd, "BackError4",   BackError4   );

      tryFilterString (aCmd, "UnityInit1",   UnityInit1    );
      tryFilterString (aCmd, "UnityInit2",   UnityInit2    );
      tryFilterString (aCmd, "UnityInit3",   UnityInit3    );
      tryFilterString (aCmd, "UnityInit4",   UnityInit4    );
      tryFilterString (aCmd, "UnityRun1",    UnityRun1     );
      tryFilterString (aCmd, "UnityRun2",    UnityRun2     );
      tryFilterString (aCmd, "UnityRun3",    UnityRun3     );
      tryFilterString (aCmd, "UnityRun4",    UnityRun4     );

      tryFilterString (aCmd, "ZCalInit1",  ZCalInit1     );
      tryFilterString (aCmd, "ZCalInit2",  ZCalInit2     );
      tryFilterString (aCmd, "ZCalInit3",  ZCalInit3     );
      tryFilterString (aCmd, "ZCalInit4",  ZCalInit4     );
      tryFilterString (aCmd, "ZCalRun1",   ZCalRun1      );
      tryFilterString (aCmd, "ZCalRun2",   ZCalRun2      );
      tryFilterString (aCmd, "ZCalRun3",   ZCalRun3      );
      tryFilterString (aCmd, "ZCalRun4",   ZCalRun4      );
      tryFilterString (aCmd, "ZCalError1", ZCalError1    );
      tryFilterString (aCmd, "ZCalError2", ZCalError2    );
      tryFilterString (aCmd, "ZCalError3", ZCalError3    );
      tryFilterString (aCmd, "ZCalError4", ZCalError4    );

      tryFilterString (aCmd, "PEst1",     PEst1         );
      tryFilterString (aCmd, "PEst2",     PEst2         );
      tryFilterString (aCmd, "PClimb",    PClimb        );
      tryFilterString (aCmd, "PSearch1",  PSearch1      );
      tryFilterString (aCmd, "PSearch2",  PSearch2      );
      tryFilterString (aCmd, "PSearch3",  PSearch3      );
      tryFilterString (aCmd, "PSearch4",  PSearch4      );
      tryFilterString (aCmd, "PSlant",    PSlant        );
      tryFilterString (aCmd, "UStage1",   UStage1       );
      tryFilterString (aCmd, "UStage2",   UStage2       );
      tryFilterString (aCmd, "UStage3",   UStage3       );
      tryFilterString (aCmd, "UStage4",   UStage4       );
      tryFilterString (aCmd, "UMove1",    UMove1        );
      tryFilterString (aCmd, "UMove2",    UMove2        );
      tryFilterString (aCmd, "UError",    UError        );
      tryFilterString (aCmd, "PFind1",    PFind1        );
      tryFilterString (aCmd, "PFind2",    PFind2        );

      tryFilterString (aCmd, "UGrid1",    UGrid1        );
      tryFilterString (aCmd, "UGrid2",    UGrid2        );
      tryFilterString (aCmd, "UGrid3",    UGrid3        );
      tryFilterString (aCmd, "UGrid4",    UGrid4        );
      tryFilterString (aCmd, "UGrid5",    UGrid5        );
      tryFilterString (aCmd, "UGrid6",    UGrid6        );
      tryFilterString (aCmd, "UGrid7",    UGrid7        );
      tryFilterString (aCmd, "UGrid8",    UGrid8        );

      tryFilterString (aCmd, "RO1",       RO1           );
      tryFilterString (aCmd, "RO2",       RO2           );
      tryFilterString (aCmd, "RO3",       RO3           );
      tryFilterString (aCmd, "RO4",       RO4           );

      // Set the stored try filter
      tryFilterSet(aCmd);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool PrintSettings::initialize(char* aSettingsFileName,char* aSection)
{ 
   // Guard
   if ((aSettingsFileName==0)||(aSection==0))
   {
      printf("ERROR PrintSettings::initialize\n");
      return false;
   }

   // Copy arguments
   strcpy(mSection,aSection);
   // Apply settings file to this executive   
   Ris::CmdLineFile tCmdLineFile;

   if (tCmdLineFile.open(aSettingsFileName))
   {
      tCmdLineFile.execute(this);
      tCmdLineFile.close();
      return true;
   }
   else
   {
      printf("PrintSettings::file open failed\n");
      return false;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace



