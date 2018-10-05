#include "stdafx.h"

#include "procoMsgHelper.h"

namespace ProtoComm
{
//****************************************************************************
//****************************************************************************
//****************************************************************************

void MsgHelper::initialize(TestMsg* aMsg)
{
   aMsg->mCode1 = 101;
   aMsg->mCode2 = 102;
   aMsg->mCode3 = 103;
   aMsg->mCode4 = 104;
}

void MsgHelper::show(TestMsg* aMsg)
{
   Prn::print(Prn::ThreadRun1, "TestMsg");
   Prn::print(Prn::ThreadRun1, "Code1      %d", aMsg->mCode1);
   Prn::print(Prn::ThreadRun1, "Code2      %d", aMsg->mCode2);
   Prn::print(Prn::ThreadRun1, "Code3      %d", aMsg->mCode3);
   Prn::print(Prn::ThreadRun1, "Code4      %d", aMsg->mCode4);
   Prn::print(Prn::ThreadRun1, "");
}

//****************************************************************************
//****************************************************************************
//****************************************************************************

void MsgHelper::initialize(FirstMessageMsg* aMsg)
{
   aMsg->mCode1 = 201;
}

void MsgHelper::show(FirstMessageMsg* aMsg)
{
   Prn::print(Prn::ThreadRun1, "FirstMsg");
   Prn::print(Prn::ThreadRun1, "Code1      %d", aMsg->mCode1);
   Prn::print(Prn::ThreadRun1, "");
}

//****************************************************************************
//****************************************************************************
//****************************************************************************

void MsgHelper::initialize(EchoRequestMsg* aMsg,int aNumWords)
{
   aMsg->mCode1 = 301;
   aMsg->mCode2 = 302;
   aMsg->mCode3 = 303;
   aMsg->mCode4 = 304;
   aMsg->mNumWords = aNumWords;
}

void MsgHelper::show(EchoRequestMsg* aMsg)
{
   Prn::print(Prn::ThreadRun1, "EchoRequestMsg");
   Prn::print(Prn::ThreadRun1, "Code1      %d", aMsg->mCode1);
   Prn::print(Prn::ThreadRun1, "");
}

//****************************************************************************
//****************************************************************************
//****************************************************************************

void MsgHelper::initialize(EchoResponseMsg* aMsg)
{
   aMsg->mCode1 = 401;
   aMsg->mCode2 = 402;
   aMsg->mCode3 = 403;
   aMsg->mCode4 = 404;
}

void MsgHelper::show(EchoResponseMsg* aMsg)
{
   Prn::print(Prn::ThreadRun1, "EchoResponseMsg");
   Prn::print(Prn::ThreadRun1, "Code1      %d", aMsg->mCode1);
   Prn::print(Prn::ThreadRun1, "");
}

//****************************************************************************
//****************************************************************************
//****************************************************************************

void MsgHelper::initialize(DataRecord* aMsg)
{
   aMsg->mCode1 = 501;
   aMsg->mCode2 = 502;
   aMsg->mCode3 = 503;
   aMsg->mCode4 = 504;
}

void MsgHelper::show(DataRecord* aMsg)
{
   Prn::print(Prn::ThreadRun1, "DataRecord");
   Prn::print(Prn::ThreadRun1, "Code1      %d", aMsg->mCode1);
   Prn::print(Prn::ThreadRun1, "Code2      %d", aMsg->mCode2);
   Prn::print(Prn::ThreadRun1, "Code3      %d", aMsg->mCode3);
   Prn::print(Prn::ThreadRun1, "Code4      %d", aMsg->mCode4);
   Prn::print(Prn::ThreadRun1, "");
}

//****************************************************************************
//****************************************************************************
//****************************************************************************

void MsgHelper::initialize(DataMsg* aMsg)
{
   aMsg->mUChar  = 0xA1;
   aMsg->mUShort = 0xBBB2;
   aMsg->mUInt   = 0xCCCCCCC3;
   aMsg->mUInt64 = 0xDDDDDDD4EEEEEEE5;
   aMsg->mChar   = 101;
   aMsg->mShort  = 10001;
   aMsg->mInt    = 1000000001;
   aMsg->mInt64  = 10000000000001;
   aMsg->mFloat  = 1.2345f;
   aMsg->mDouble = 2.34567;
   aMsg->mBool   = false;


   strcpy(aMsg->mString1,"String1");
   strcpy(aMsg->mString2,"String2");

   initialize(&aMsg->mDataRecord);
}

void MsgHelper::show(DataMsg* aMsg)
{
   Prn::print(Prn::ThreadRun1, "DataMsg");
   Prn::print(Prn::ThreadRun1, "UShort     %X",    aMsg->mUShort);
   Prn::print(Prn::ThreadRun1, "UInt       %X",    aMsg->mUInt);
   Prn::print(Prn::ThreadRun1, "UInt64     %llX",  aMsg->mUInt64);
   Prn::print(Prn::ThreadRun1, "Char       %d",    aMsg->mChar);
   Prn::print(Prn::ThreadRun1, "Short      %d",    aMsg->mShort);
   Prn::print(Prn::ThreadRun1, "Int        %d",    aMsg->mInt);
   Prn::print(Prn::ThreadRun1, "Int64      %lld",  aMsg->mInt64);
   Prn::print(Prn::ThreadRun1, "Float      %f",    aMsg->mFloat);
   Prn::print(Prn::ThreadRun1, "Double     %f",    aMsg->mDouble);
   Prn::print(Prn::ThreadRun1, "Bool       %d",    aMsg->mBool);

   Prn::print(Prn::ThreadRun1, "String1    %s",    aMsg->mString1);
   Prn::print(Prn::ThreadRun1, "String2    %s",    aMsg->mString2);

   show(&aMsg->mDataRecord);
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
}//namespace
