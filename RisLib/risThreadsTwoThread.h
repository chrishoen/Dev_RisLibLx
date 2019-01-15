#pragma once

/*==============================================================================
Two Thread, a thread construct that has a short term thread and a long term
thread. The short term thread executes qcalls that have a short duration
and the long term thread executes qcalls that can have long durations. Many
short term qcalls can execute while one long term qcall executes. It can be
used as a thread that can send a command somewhere and wait for a response.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <functional>

#include "risThreadsQCallThread.h"
#include "risThreadsNotify.h"

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
//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a thread that executes short term qcalls. That is, execution of its
// thread qcalls is expected to take a short amount of time.

class TwoThreadShortThread : public Ris::Threads::BaseQCallThread
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Thread call pointers, these are called by the thread base overloads, if 
   // they are bound. They are bound to functions by the instantiator before
   // the thread is launched. Any that are not bound result in a no op for the
   // thread run function.
   std::function<void(void)>   mThreadInitCallPointer;
   std::function<void(void)>   mThreadExitCallPointer;
   std::function<void(int)>    mThreadExecuteOnTimerCallPointer;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Constructor.
   TwoThreadShortThread();
  ~TwoThreadShortThread();

   // Thread init function. This is called by the base class immedidately 
   // after the thread starts running.
   // It calls the associated call pointer, if it exists.
   void threadInitFunction() override;

   // Thread exit function. This is called by the base class immedidately
   //  before the thread is terminated.
   // It calls the associated call pointer, if it exists.
   void threadExitFunction() override;

   // Execute periodically. This is called by the base class timer.
   // It calls the associated call pointer, if it exists.
   void executeOnTimer(int aTimerCount) override;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a thread that executes long term qcalls. That is, execution of
// thread qcalls is expected to take a long amount of time.

class TwoThreadLongThread : public Ris::Threads::BaseQCallThread
{
public:
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Base class for a two thread. It has a short term thread and a long term
// thread.

class BaseTwoThread
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // The two threads. 
   TwoThreadShortThread*  mShortThread;
   TwoThreadLongThread*   mLongThread;

   // Notification object. The long thread waits on notifications. The
   // short thread sends notifications.
   Notify mNotify;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // If true then the shutdown order is long,short. If false then the
   // shutdown order is short,long.
   bool mShutdownLongShort;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor
   BaseTwoThread();
   virtual ~BaseTwoThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Launch, shutdown the two threads.
   void launchThreads(); 
   virtual void shutdownThreads();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. These are overloaded by inheritors. They execute in the context
   // of the short thread. They are called by the associated short term thread
   // functions.

   virtual void threadInitFunction(){}
   virtual void threadExitFunction(){}
   virtual void executeOnTimer(int aTimerCount){}

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Show thread state info.
   virtual void showThreadInfo();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace


