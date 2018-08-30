#pragma once
/*==============================================================================
These types are intended to be portable across different
software development environments.
==============================================================================*/

// Win32
namespace Ris
{

// microsoft specific
#if defined(WIN32)

typedef void*    HandleT;
typedef unsigned SocketT;

// gcc specific
#else

typedef int      HandleT;
typedef int      SocketT;

#endif
}

