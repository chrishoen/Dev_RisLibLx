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

   // Current time count, incremented on every timer event.
   // Counts the number of timer events that have occurred
   // since the timer was created.
   int  mTimerCount;

   // Return true if the previous wait unblocking was a result of the timer 
   // or the event.
   bool mWasTimerFlag;
   bool mWasEventFlag;

   // Return true if the object is valid.
   bool mValidFlag;

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

   // Start a timer periodically, in milliseconds.
   void initialize(int aTimerPeriod);

   // Stop the timer.
   void finalize();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Post to the event.
   void postEvent();

   // Wait for the timer or event.
   void waitForTimerOrEvent();

   // Return true if the previous wait unblocking was a result of the timer 
   // or the event.
   bool wasTimer();
   bool wasEvent();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace


