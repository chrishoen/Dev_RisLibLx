/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <pthread.h> 

#include "risLCCriticalSection.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

void chkerror(int aRet, char* aLabel)
{
   if (aRet == 0)return;
   printf("FAIL %s %d\n", aLabel, aRet);
   exit(1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Created a critical section. Pass the returned code to the following
// functions.

void* createCriticalSection()
{
   int ret;

   pthread_mutex_t* tMutex = new pthread_mutex_t;
   ret = pthread_mutex_init(tMutex, NULL);
   chkerror(ret, "pthread_mutex_init");

   return (void*)tMutex;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Enter a critical section. This is used to lock a resource for a short
// time interval.

void enterCriticalSection(void* aCriticalSection)
{
   int ret;

   ret = pthread_mutex_lock((pthread_mutex_t*)aCriticalSection);
   chkerror(ret, "pthread_mutex_lock");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Leave a critical section. This is used to unlock a resource.

void leaveCriticalSection(void* aCriticalSection)
{
   int ret;

   ret = pthread_mutex_unlock((pthread_mutex_t*)aCriticalSection);
   chkerror(ret, "pthread_mutex_unlock");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Destroy a critical section.

void destroyCriticalSection(void* aCriticalSection)
{
   int ret;

   ret = pthread_mutex_destroy((pthread_mutex_t*)aCriticalSection);
   chkerror(ret, "pthread_mutex_destroy");

   delete aCriticalSection;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

