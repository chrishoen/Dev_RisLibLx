#ifndef _LFINDEX_H_
#define _LFINDEX_H_


#include "my_defs.h"
#include <atomic>

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
// Lock Free Index used by CAS based algorithms.

__declspec(align(8))
struct LFIndex
{
   // Used as a node index.
   int mIndex;
   // Used to overcome the ABA problem.
   int mCount;

   // Constructor
   LFIndex() noexcept
   {
      mIndex = 0;
      mCount = 0;
   }

   // Constructor
   LFIndex(int aIndex, int aCount) noexcept
   {
      mIndex = aIndex;
      mCount = aCount;
   }
};

// Operator
inline bool operator==(const LFIndex& lhs, const LFIndex& rhs)
{
   return lhs.mIndex == rhs.mIndex && lhs.mCount == rhs.mCount;
}

// Operator
inline bool operator!=(const LFIndex& lhs, const LFIndex& rhs)
{
   return lhs.mIndex != rhs.mIndex && lhs.mCount != rhs.mCount;
}

// Atomic
typedef std::atomic<LFIndex> AtomicLFIndex;

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Base class for lock free container blocks. This is inherited by classes that
// are contained in intrusive lock free containers.

class BaseLFBlock
{
public:
   int mLFNodeIndex;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
#endif