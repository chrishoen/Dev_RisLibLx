#pragma once

/*==============================================================================
These are analagous to digital logic components.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <atomic>

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{
namespace Logic
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This class provides an AND OR masked latch.
// It has a mask array and a latch array and some condition flags.
// Initially mask bits are set and over time latch bits are set.
// The masked latch bits are ANDed and ORed to set the condition flags
// that are used to indicate if ANY or ALL masked latch bits are set.
// ANY is the OR  of all of the masked latch bits.
// ALL is the AND of all of the masked latch bits.
//
// For example:
//
//    reset();
//
//    setMask(2,true);
//    setMask(3,true);
//
//    setLatch(2,false);
//    setLatch(3,true);
//    
//    isAny() returns true
//    isAll() returns false
//
//    setLatch(2,true);
//    setLatch(3,true);
//    
//    isAny() returns true
//    isAll() returns true
//

class AndOrLatch
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   std::atomic<unsigned>  mLatch;
   std::atomic<unsigned>  mMask;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Intrastructure.

   // Constructor.
   AndOrLatch();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Resets.
   void reset();
   void resetMask();
   void resetLatch();

   // Mask and latch.
   void setMaskBit    (int aBitNum);
   void clearMaskBit  (int aBitNum);
   void setLatchBit   (int aBitNum);
   void clearLatchBit (int aBitNum);

   // Return true if any masked latch value  is true.
   bool isAny();
   // Return true if all masked latch values are true.
   bool isAll();
  
};
//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace
