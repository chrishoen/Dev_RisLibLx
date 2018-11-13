#pragma once

/*==============================================================================
Thread waitable timer and events.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// The class provides a mechanism that allows a thread to wait on a timer
// and on a set of events.


class Waitable
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

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
   // Methods.

   // Constructor.
   Waitable();
  ~Waitable();

   // Start a timer periodically, in milliseconds
   void initialize(int aTimerPeriod);

   // Stop the timer.
   void finalize();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Wait for the timer or event.
   void waitForTimerOrEvent();

   // Post to the event. This will unblock any pending waits.
   void postEvent();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace


