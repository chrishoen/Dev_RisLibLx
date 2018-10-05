#pragma once

#include "procoMsg.h"

namespace ProtoComm
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

class MsgHelper
{
public:

   static void initialize (TestMsg* aMsg);
   static void initialize (FirstMessageMsg* aMsg);
   static void initialize (EchoRequestMsg* aMsg,int aNumWords = 0);
   static void initialize (EchoResponseMsg* aMsg);

   static void initialize (DataRecord* aMsg);
   static void initialize (DataMsg* aMsg);

   static void show (TestMsg* aMsg);
   static void show (FirstMessageMsg* aMsg);
   static void show (EchoRequestMsg* aMsg);
   static void show (EchoResponseMsg* aMsg);
   static void show (DataRecord* aMsg);
   static void show (DataMsg* aMsg);

};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
