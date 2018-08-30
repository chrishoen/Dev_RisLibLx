#pragma once

/*==============================================================================

QCall (Queued Function Call) 

A QCall is a function call that is invoked from a calling thread to a
called thread. The calling thread encapsulates a function call and queues
it to the called thread. The called thread dequeues the function call and
executes it in its own context.

This file contains class definitions for:
1) QCalls
2) A base class for threads that process QCalls 

class BaseQCall
class QCall0
class QCall1 <class X1>
class QCall2 <class X1,class X2>
class QCall3 <class X1,class X2,class X3>
class QCall4 <class X1,class X2,class X3,class X4>
class BaseQCallThreadEx

A QCall (Queued Function Call) is a thread interface mechanism that allows 
thread member functions to be called by other threads in a deferred manner
such that execution of the member functions is serialized and takes place
in the context of the called thread, not in the context of the calling thread.

A QCall is an object that contains data about the member function to execute
and argument data for the member function. A QCall encapsualtes a function
and its arguments.

Threads that can receive QCalls are based upon a single mCallQue to which QCalls
are sent by calling threads. When a QCall is sent to the mCallQue, the thread 
wakes up and processes it by executing the thread member function indicated
by the QCall.

This is similar to a message processing based thread that contains a single
message queue to which messages are sent, but it has the advantage of having
a simplified function call interface for each message.
 
QCalls are implemented with code based on CallPointers and templates.
CallPointers are templated constructs that implement C++ function pointers
that can point to class/object member functions. They act like c function
pointers, but can be used with class member functions.

QCalls contain three things:

1) An "invoke" CallPointer that is used to transfer a QCall to a specific
thread's QCallQue. It contains the address of a thread's putQCallToThread method.
2) An "execute" CallPointer that is used to execute the thread member
function (the procedure call). It contains the address of the member function.
3) The arguments to the thread member function (the arguments 
to the procedure call)

QCalls are organized around a QCall base class and class templates that manage
the different function signatures ("execute" CallPointers).

BaseQCall is the base class

QCall0 is a class template for a QCall with 0 arguments, no return
QCall1 is a class template for a QCall with 1 argument,  no return
QCall2 is a class template for a QCall with 2 arguments, no return
QCall3 is a class template for a QCall with 3 argument,  no return

The template syntax is based on that of the CallPointers.

Note that CallPointers that have returns cannot be used because there
is no way to get the return back to the caller.

QCall threads inherit from BaseQCallThreadEx or BaseQCallThreadExMultipleWait.

BaseQCallThreadEx contains an mCallQue and a threadRunFunction that services
it. The mCallQue is a queue of QCall pointers that contains a semaphore that 
the thread waits at. When a QCall pointer is written to the queue, the thread
wakes up, gets the QCall from the queue and and calls the QCall "execute" 
CallPointer, passing in the QCall arguments.

BaseQCallThreadEx does a WaitForSingleObject on the QCallQue semaphore. The
wait has a one second timeout that is used to poll a thread terminate flag
that is used to shutdown the thread.

BaseQCallThreadExMultipleWait is similar to BaseQCallThreadEx. It does a
WaitForMultipleObject, where one of the objects is the mCallQue semaphore.
Other objects are a timer and a termination request semaphore. Inheriting
threads can add other objects to to the multiple wait list.

QCalls are used in the following manner:

1) The called thread contains a member variable that is a QCall. Its 
   mInvokeCallPointer contains the address of the thread's putQCallToThread
   method. Its mExecuteCallPointer contains the address of the deferred 
   procedure member function.

2) The calling thread invokes the QCall by calling its "invoke" method, 
   passing in procedure arguments. The "invoke" method creates a new copy
   of the QCall on the heap, sets its arguments, and calls the QCall 
   mInvokeCallPointer. This writes the address of the QCall to the called 
   thread's mCallQue and signals the mCallQue semaphore.

3) The called thread's threadRunFunction is waiting on its mCallQue. 
   It wakes up, gets the address of the QCall from the mCallQue, and calls
   the QCall mExecuteCallPointer. This executes the thread member function
   associated with the QCall. Then it deletes the QCall from the heap.

For an example, the called thread could have a QCall and two methods such as:

   void invokeSomeFunction  (int aSessionIndex, Message* aRxMsg);
   void executeSomeFunction (int aSessionIndex, Message* aRxMsg);
   typedef Ris::Threads::QCall2<int,Message*> SomeFunctionQCall;
   SomeFunctionQCall mSomeFunctionQCall;

The calling thread invokes the QCall by:

  gCalledThread->invokeSomeFunction(1,mRxMsg);
                        or
  gCalledThread->mSomeFunctionQCall.invoke(1,mRxMsg);


The calling thread calls invokeSomeFunction in its context and the called
thread calls executeSomeFunction in its context.

Note that what happens here is that mSomeFunctionQCall contains a CallPointer
to executeSomeFunction. When it is invoked, it makes a copy of itself on the
heap (with correct arguments). The copy is passed to the mCallQue and is
executed by the thread run function and then deleted.
 
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risPortableTypes.h"
#include "risCallPointer.h"
#include "risThreadsThreads.h"
#include "risThreadsTimer.h"
#include "risThreadsSynch.h"
#include "risThreadsQCall.h"

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
// BaseQCallThreadEx is a thread that waits on a queue of QCalls, which are
// objects that contain a pointer to a function and values for arguments that
// are to be passed to that function. This is similar to a thread that waits
// on a message queue, and wakes up and processes messages when they are sent
// to it's queue. Here the messages take the form of QCalls. 
//
// BaseQCallThreadEx contains an mCallQue and a threadRunFunction that services
// it. The mCallQue is a queue of QCall pointers and it contains a semaphore that 
// the thread waits at. When a QCall pointer is written to the queue, the thread
// wakes up, gets the QCall from the queue and and calls the QCall "execute" 
// CallPointer, passing in the QCall arguments.
// 
// BaseQCallThreadEx supplies a shutdownThread that invokes a mTerminateQCall that
// sets an mTerminateFlag. The threadRunFunction polls the mTerminateFlag
// and exits the thread if it is true.

class BaseQCallThread : public Ris::Threads::BaseThread,public BaseQCallTarget
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // If true the the thread will terminate at the next post to the central
   // semaphore.
   bool mTerminateFlag;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members, call queue.

   // This semahore is posted to when QCalls for this thread are invoked or
   // when a periodic timer event occurrs.
   CountingSemaphore mCentralSem;

   // Inherited target call queue size.
   int  mCallQueSize;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members, thread timer.

   // Thread timer. It periodically executes threadExecuteOnTimer,
   // not in the execution context of this thread.
   ThreadTimer    mThreadTimer;
   TimerCall      mThreadTimerCall;
   
   // Set by threadExecuteOnTimer to indicate that a timer
   // update has occurred.
   bool  mTimerExecuteFlag;

   // Timer period in milliseconds.
   // If this is zero then no timer is created.
   int   mTimerPeriod;

   // These give the number of timer events that have
   // occurred since thread launch
   int   mCurrentTimeCount;
   int   mTimerCurrentTimeCount;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members, status.

   // If true then abort the qcall.
   bool mQCallAbortFlag;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Intrastructure.

   // Constructor.
   BaseQCallThread();
  ~BaseQCallThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods, thread base class overloads:

   // Initialize the thread call queue.
   void threadResourceInitFunction() override;

   // Initialize the thread timer.
   void threadTimerInitFunction() override; 

   // Execute a loop that calls threadRunRecursive to process the call queue.
   // The loop terminates on the mTerminateFlag. It waits for the call queue
   // semaphore, extracts a call from the call queue, and executes the call. 
   void threadRunFunction() override; 

   // Finalize the thread call queue.
   void threadResourceExitFunction() override;

   // Set the mTerminateFlag and post to the call semaphore. Then wait for
   // the thread to terminate.
   void shutdownThread()override; 

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods, qcall target overloads:

   // Post to the central semahore. It is called by QCall invokations after
   // a QCall has been enqueued to the call queue.
   void notifyQCallAvailable() override;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods, inheritor overloads:

   // This is executed by the timer. It updates timer variables
   // and signals the central semaphore to wake up the thread.
   // aCurrentTimeCount gives the number of timer events that
   // have occurred since thread launch.
   virtual void threadExecuteOnTimer(int aCurrentTimeCount);

   // Inheritors provide an overload for this.
   // It is executed in the context of this thread, after the
   // central semaphore wakes up.
   // aCurrentTimeCount gives the number of timer events that
   // have occurred since thread launch.
   // It is called by threadRunFunction and is caused by
   // threadExecuteOnTimer.
   virtual void executeOnTimer(int aCurrentTimeCount){}
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace
