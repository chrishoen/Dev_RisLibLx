#pragma once

/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// These functions provide a synchronization lock that is used to protect
// access to critical sections of code. These should be used around short
// sections that do not block. These functions are intended to be cross
// platform. They use a void* general purpose variable.

// Created a critical section. Pass the returned code to the following
// functions.
void* createCriticalSection();

// Enter a critical section. This is used to lock a resource for a short
// time interval.
void enterCriticalSection(void* aCriticalSection);

// Leave a critical section. This is used to unlock a resource.
void leaveCriticalSection(void* aCriticalSection);

// Destroy a critical section.
void destroyCriticalSection(void* aCriticalSection);

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace


