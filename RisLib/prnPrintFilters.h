#pragma once
/*==============================================================================
Filter constants used by the print facility.
==============================================================================*/

namespace Prn
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constants.

   static const int cFilterTableSize   = 300;

   static const int  FilterZero        =   0;
   static const int  PrintInit1        =   1;
   static const int  PrintInit2        =   2;

   static const int  PrintInit3        =   3;
   static const int  PrintInit4        =   4;
   static const int  PrintRun1         =   5;
   static const int  PrintRun2         =   6;
   static const int  PrintRun3         =   7;
   static const int  PrintRun4         =   8;

   static const int  SocketInit1       =   9;
   static const int  SocketInit2       =  10;
   static const int  SocketInit3       =  11;
   static const int  SocketInit4       =  12;
   static const int  SocketRun1        =  13;
   static const int  SocketRun2        =  14;
   static const int  SocketRun3        =  15;
   static const int  SocketRun4        =  16;
   static const int  SocketError1      =  17;
   static const int  SocketError2      =  18;
   static const int  SocketError3      =  19;
   static const int  SocketError4      =  20;

   static const int  SerialInit1       =  21;
   static const int  SerialInit2       =  22;
   static const int  SerialInit3       =  23;
   static const int  SerialInit4       =  24;
   static const int  SerialRun1        =  25;
   static const int  SerialRun2        =  26;
   static const int  SerialRun3        =  27;
   static const int  SerialRun4        =  28;
   static const int  SerialError1      =  29;
   static const int  SerialError2      =  30;
   static const int  SerialError3      =  31;
   static const int  SerialError4      =  32;

   static const int  ThreadInit1       =  37;
   static const int  ThreadInit2       =  38;
   static const int  ThreadInit3       =  39;
   static const int  ThreadInit4       =  40;
   static const int  ThreadRun1        =  41;
   static const int  ThreadRun2        =  42;
   static const int  ThreadRun3        =  43;
   static const int  ThreadRun4        =  44;
   static const int  ProcInit1         =  45;
   static const int  ProcInit2         =  46;
   static const int  ProcInit3         =  47;
   static const int  ProcInit4         =  48;
   static const int  ProcRun1          =  49;
   static const int  ProcRun2          =  50;
   static const int  ProcRun3          =  51;
   static const int  ProcRun4          =  52;
   static const int  PointInit1        =  53;
   static const int  PointInit2        =  54;
   static const int  PointInit3        =  55;
   static const int  PointInit4        =  56;
   static const int  PointRun1         =  57;
   static const int  PointRun2         =  58;
   static const int  PointRun3         =  59;
   static const int  PointRun4         =  60;
   static const int  QCallInit1        =  61;
   static const int  QCallInit2        =  62;
   static const int  QCallInit3        =  63;
   static const int  QCallInit4        =  64;
   static const int  QCallRun1         =  65;
   static const int  QCallRun2         =  66;
   static const int  QCallRun3         =  67;
   static const int  QCallRun4         =  68;
   static const int  CalcInit1         =  69;
   static const int  CalcInit2         =  70;
   static const int  CalcInit3         =  71;
   static const int  CalcInit4         =  72;
   static const int  CalcRun1          =  73;
   static const int  CalcRun2          =  74;
   static const int  CalcRun3          =  75;
   static const int  CalcRun4          =  76;
   static const int  CalcError1        =  77;
   static const int  CalcError2        =  78;
   static const int  CalcError3        =  79;
   static const int  CalcError4        =  80;
   static const int  MsgRx1            =  81;
   static const int  MsgRx2            =  82;
   static const int  MsgRx3            =  83;
   static const int  MsgRx4            =  84;
   static const int  ViewInit1         =  85; 
   static const int  ViewInit2         =  86;
   static const int  ViewInit3         =  87;
   static const int  ViewInit4         =  88;
   static const int  ViewRun1          =  89;
   static const int  ViewRun2          =  90;
   static const int  ViewRun3          =  91;
   static const int  ViewRun4          =  92;
   static const int  CamInit1          =  93;
   static const int  CamInit2          =  94;
   static const int  CamInit3          =  95;
   static const int  CamInit4          =  96;
   static const int  CamRun1           =  97;
   static const int  CamRun2           =  98;
   static const int  CamRun3           =  99;
   static const int  CamRun4           = 100;
   static const int  CamError1         = 101;
   static const int  CamError2         = 102;
   static const int  CamError3         = 103;
   static const int  CamError4         = 104;
   static const int  SensorInit1       = 105;
   static const int  SensorInit2       = 106;
   static const int  SensorInit3       = 107;
   static const int  SensorInit4       = 108;
   static const int  SensorRun1        = 109;
   static const int  SensorRun2        = 110;
   static const int  SensorRun3        = 111;
   static const int  SensorRun4        = 112;
   static const int  SensorError1      = 113;
   static const int  SensorError2      = 114;
   static const int  SensorError3      = 115;
   static const int  SensorError4      = 116;
   static const int  DspInit1          = 117;
   static const int  DspInit2          = 118;
   static const int  DspInit3          = 119;
   static const int  DspInit4          = 120;
   static const int  DspRun1           = 121;
   static const int  DspRun2           = 122;
   static const int  DspRun3           = 123;
   static const int  DspRun4           = 124;
   static const int  DspError1         = 125;
   static const int  DspError2         = 126;
   static const int  DspError3         = 127;
   static const int  DspError4         = 128;
   static const int  Result1           = 129;
   static const int  Result2           = 130;
   static const int  Result3           = 131;
   static const int  Result4           = 132;
   static const int  Result5           = 133;
   static const int  Result6           = 134;
   static const int  Result7           = 135;
   static const int  Result8           = 136;
   static const int  Cal1              = 137;
   static const int  Cal2              = 138;
   static const int  Cal3              = 139;
   static const int  Cal4              = 140;
   static const int  Cal5              = 141;
   static const int  Cal6              = 142;
   static const int  Cal7              = 143;
   static const int  Cal8              = 144;
   static const int  CalError1         = 145;
   static const int  CalError2         = 146;
   static const int  CalError3         = 147;
   static const int  CalError4         = 148;
   static const int  BackInit1         = 149;
   static const int  BackInit2         = 150;
   static const int  BackInit3         = 151;
   static const int  BackInit4         = 152;
   static const int  BackRun1          = 153;
   static const int  BackRun2          = 154;
   static const int  BackRun3          = 155;
   static const int  BackRun4          = 156;
   static const int  BackError1        = 157;
   static const int  BackError2        = 158;
   static const int  BackError3        = 159;
   static const int  BackError4        = 160;
   static const int  UnityInit1        = 161;
   static const int  UnityInit2        = 162;
   static const int  UnityInit3        = 163;
   static const int  UnityInit4        = 164;
   static const int  UnityRun1         = 165;
   static const int  UnityRun2         = 166;
   static const int  UnityRun3         = 167;
   static const int  UnityRun4         = 168;
   static const int  ZCalInit1         = 169;
   static const int  ZCalInit2         = 170;
   static const int  ZCalInit3         = 171;
   static const int  ZCalInit4         = 172;
   static const int  ZCalRun1          = 173;
   static const int  ZCalRun2          = 174;
   static const int  ZCalRun3          = 175;
   static const int  ZCalRun4          = 176;
   static const int  ZCalError1        = 177;
   static const int  ZCalError2        = 178;
   static const int  ZCalError3        = 179;
   static const int  ZCalError4        = 180;
   static const int  PEst1             = 181;
   static const int  PEst2             = 182;
   static const int  PClimb            = 183;
   static const int  PSearch1          = 184;
   static const int  PSearch2          = 185;
   static const int  PSearch3          = 186;
   static const int  PSearch4          = 187;
   static const int  PSlant            = 188;
   static const int  UStage1           = 189;
   static const int  UStage2           = 190;
   static const int  UStage3           = 191;
   static const int  UStage4           = 192;
   static const int  UMove1            = 193;
   static const int  UMove2            = 194;
   static const int  UError            = 195;
   static const int  PFind1            = 196;
   static const int  PFind2            = 197;
   static const int  UGrid1            = 198;
   static const int  UGrid2            = 199;
   static const int  UGrid3            = 200;
   static const int  UGrid4            = 201;
   static const int  UGrid5            = 202;
   static const int  UGrid6            = 203;
   static const int  UGrid7            = 204;
   static const int  UGrid8            = 205;
   static const int  RO1               = 206;
   static const int  RO2               = 207;
   static const int  RO3               = 208;
   static const int  RO4               = 209;

   static const int  FileInit1 = 220;
   static const int  FileInit2 = 221;
   static const int  FileInit3 = 222;
   static const int  FileInit4 = 223;
   static const int  FileRun1 = 224;
   static const int  FileRun2 = 225;
   static const int  FileRun3 = 226;
   static const int  FileRun4 = 227;
   static const int  FileError1 = 228;
   static const int  FileError2 = 229;
   static const int  FileError3 = 230;
   static const int  FileError4 = 231;

   //******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

