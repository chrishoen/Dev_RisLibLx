#ifndef _MY_DEFS_H_
#define _MY_DEFS_H_

/*==============================================================================
Description:
My definitions for physical constants
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#define MY_PI  3.14159265358979323846264338327950288419716939937510
#define MY_2PI 2.0*MY_PI
#define MY_C   299,792,458 
#define MY_RAD   MY_PI/180.0
#define MY_DEG   180.0/MY_PI


/*
#ifndef _MSC_VER
#define NOEXCEPT noexcept(true)
#else
#define NOEXCEPT
#endif
*/

#define NOEXCEPT noexcept(true)

// operator new[] adds extra bytes to store number of elements.
static const int cNewArrayExtraMemory = 4;



#endif

