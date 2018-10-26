#pragma once

/*==============================================================================
Thread timer.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include <functional>

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// The Timer class provides a timer that can be used to call a function
// periodically. It is passed a function pointer and a timer period.


// Call pointer for function to be called periodically
typedef std::function<void(int)> TimerCall;

class ThreadTimer
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Pointer to function to execute periodically
   TimerCall  mTimerCall;

   // Period, in milliseconds
   int  mTimerPeriod;

   // Current time count, incremented on every timer event.
   // Counts the number of timer events that have occurred
   // since the timer was created.
   int  mCurrentTimeCount;

protected:
   // Pimpl pattern. Used to hide details of the operating system specific
   // variables, like the timer handle, from the .h file so that this
   // include file can be complied by different compliers. The class is
   // defined in the .cpp file, where there is a different version for
   // different compilers.
   class Specific;
   Specific* mSpecific;

public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Intrastructure.

   // Constructor.
   ThreadTimer();
  ~ThreadTimer();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Execute a timer call periodically, in milliseconds
   void startTimer (TimerCall aTimerCall,int aTimerPeriod);

   // Cancel the timer call.
   void cancel();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace


