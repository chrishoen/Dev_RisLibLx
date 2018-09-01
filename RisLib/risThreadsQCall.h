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
class BaseQCallThread

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

QCall threads inherit from BaseQCallThread or BaseQCallThreadMultipleWait.

BaseQCallThread contains an mCallQue and a threadRunFunction that services
it. The mCallQue is a queue of QCall pointers that contains a semaphore that 
the thread waits at. When a QCall pointer is written to the queue, the thread
wakes up, gets the QCall from the queue and and calls the QCall "execute" 
CallPointer, passing in the QCall arguments.

BaseQCallThread does a WaitForSingleObject on the QCallQue semaphore. The
wait has a one second timeout that is used to poll a thread terminate flag
that is used to shutdown the thread.

BaseQCallThreadMultipleWait is similar to BaseQCallThread. It does a
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
#include <new>
#include "risLMPacketQueue.h"
#include "risCallPointer.h"

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// QCall target base class. Inheriting classes process received QCalls.

class BaseQCall;

class BaseQCallTarget
{
public:

   // Lock free queue of fixed size blocks, contains QCalls.
   // QCall invokations enqueue QCalls to this queue.
   // QCall targets dequeue from it.

   LMPacketQueue mCallQueue;

   // Target inheritors provide an override for this method.
   // It is called after a QCall has been enqueued to the target queue.
   // It notifies the target that a QCall is available.

   virtual void notifyQCallAvailable()=0;

   // Call queue block size, this is the maximum size of a QCall.
   static const int cCallQueueBlockSize = 128;

   // Initialize the call queue
   void initializeCallQueue(int aCallQueueSize)
   {
      mCallQueue.initialize(aCallQueueSize,cCallQueueBlockSize);
   }

   // Finalize the call queue
   void finalizeCallQueue()
   {
      mCallQueue.finalize();
   }
};


//******************************************************************************
//******************************************************************************
//******************************************************************************
// QCall

//******************************************************************************
// Base class

class  BaseQCall
{
public:
   //---------------------------------------------------------------------------
   // Pointer to the target that the QCall is bound to.

   BaseQCallTarget* mTarget;

   //---------------------------------------------------------------------------
   // Execute, it is called by the target thread to execute the QCall. 

   virtual void execute()=0;

   //---------------------------------------------------------------------------
   // Constructor.

   BaseQCall()
   {
      mTarget=0;
   }

   bool isValid(){return mTarget !=0;}
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// QCall0 is a class template for a QCall with no arguments

class  QCall0 : public BaseQCall
{
public:
   //---------------------------------------------------------------------------
   // Function call overload. It enqueues a copy of this QCall to the target
   // queue and then notifies the target that a QCall is available.

   void operator()()
   {
      // Start a write to the target queue, obtaining memory for a new QCall.
      int tIndex;
      void* tBlock = mTarget->mCallQueue.startWrite(&tIndex);
      if (tBlock==0) return;
      // Create a copy of this QCall, using the new memory.
      QCall0* tQCall = new(tBlock)QCall0(*this);
      // Finish the write to the target queue.
      mTarget->mCallQueue.finishWrite(tIndex);
      // Notify the target.
      mTarget->notifyQCallAvailable();
   }

   //---------------------------------------------------------------------------
   // Execute, it is called by the target thread to execute the QCall. 

   // Execute CallPointer, it contains the address of the function to be called.
   // This typically contains the address of a target class member function.
   typedef Ris::CallPointer0<> ExecuteCallPointer;
   ExecuteCallPointer mExecuteCallPointer;

   // This is called by the target thread, after it dequeues the QCall.
   void execute()
   {
      mExecuteCallPointer();
   }

   //---------------------------------------------------------------------------
   // Bind a target to the QCall.

   template <class TargetObject,class CallMethod>
   void bind(TargetObject aTargetObject,CallMethod aCallMethod)
   {
      mTarget = aTargetObject;
      mExecuteCallPointer.bind (aTargetObject,aCallMethod);
   }

   template <class TargetObject,class CallObject,class CallMethod>
   void bind(TargetObject aTargetObject,CallObject aCallObject,CallMethod aCallMethod)
   {
      mTarget = aTargetObject;
      mExecuteCallPointer.bind (aCallObject,aCallMethod);
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// QCall1 is a class template for a QCall with 1 argument

template <class X1>
class  QCall1 : public BaseQCall
{
public:
   //---------------------------------------------------------------------------
   // Queued procedure call arguments:

   X1 mX1;

   //---------------------------------------------------------------------------
   // Function call overload. It enqueues a copy of this QCall to the target
   // queue and then notifies the target that a QCall is available.

   void operator()(X1 aX1)
   {
      // Store arguments into this QCall.
      mX1=aX1;
      // Start a write to the target queue, obtaining memory for a new QCall.
      int tIndex;
      void* tBlock = mTarget->mCallQueue.startWrite(&tIndex);
      if (tBlock==0) return;
      // Create a copy of this QCall, using the new memory.
      QCall1* tQCall = new(tBlock)QCall1(*this);
      // Finish the write to the target queue.
      mTarget->mCallQueue.finishWrite(tIndex);
      // Notify the target.
      mTarget->notifyQCallAvailable();
   }

   //---------------------------------------------------------------------------
   // Execute, it is called by the target thread to execute the QCall. 

   // Execute CallPointer, it contains the address of the function to be called.
   // This typically contains the address of a target class member function.
   typedef Ris::CallPointer1<X1> ExecuteCallPointer;
   ExecuteCallPointer mExecuteCallPointer;

   // This is called by the target thread, after it dequeues the QCall.
   void execute()
   {
      mExecuteCallPointer(mX1);
   }

   //---------------------------------------------------------------------------
   // Bind a target to the QCall.

   template <class TargetObject,class CallMethod>
   void bind(TargetObject aTargetObject,CallMethod aCallMethod)
   {
      mTarget = aTargetObject;
      mExecuteCallPointer.bind (aTargetObject,aCallMethod);
   }

   template <class TargetObject,class CallObject,class CallMethod>
   void bind(TargetObject aTargetObject,CallObject aCallObject,CallMethod aCallMethod)
   {
      mTarget = aTargetObject;
      mExecuteCallPointer.bind (aCallObject,aCallMethod);
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// QCall2 is a class template for a QCall with 2 arguments

template <class X1,class X2>
class  QCall2 : public BaseQCall
{
public:
   //---------------------------------------------------------------------------
   // Queued procedure call arguments:

   X1 mX1;
   X2 mX2;

   //---------------------------------------------------------------------------
   // Function call overload. It enqueues a copy of this QCall to the target
   // queue and then notifies the target that a QCall is available.

   void operator()(X1 aX1,X2 aX2)
   {
      // Store arguments into this QCall.
      mX1=aX1;
      mX2=aX2;
      // Start a write to the target queue, obtaining memory for a new QCall.
      int tIndex;
      void* tBlock = mTarget->mCallQueue.startWrite(&tIndex);
      if (tBlock==0) return;
      // Create a copy of this QCall, using the new memory.
      QCall2* tQCall = new(tBlock)QCall2(*this);
      // Finish the write to the target queue.
      mTarget->mCallQueue.finishWrite(tIndex);
      // Notify the target.
      mTarget->notifyQCallAvailable();
   }

   //---------------------------------------------------------------------------
   // Execute, it is called by the target thread to execute the QCall. 

   // Execute CallPointer, it contains the address of the function to be called.
   // This typically contains the address of a target class member function.
   typedef Ris::CallPointer2<X1,X2> ExecuteCallPointer;
   ExecuteCallPointer mExecuteCallPointer;

   // This is called by the target thread, after it dequeues the QCall.
   void execute()
   {
      mExecuteCallPointer(mX1,mX2);
   }

   //---------------------------------------------------------------------------
   // Bind a target to the QCall.

   template <class TargetObject,class CallMethod>
   void bind(TargetObject aTargetObject,CallMethod aCallMethod)
   {
      mTarget = aTargetObject;
      mExecuteCallPointer.bind (aTargetObject,aCallMethod);
   }

   template <class TargetObject,class CallObject,class CallMethod>
   void bind(TargetObject aTargetObject,CallObject aCallObject,CallMethod aCallMethod)
   {
      mTarget = aTargetObject;
      mExecuteCallPointer.bind (aCallObject,aCallMethod);
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// QCall3 is a class template for a QCall with 3 arguments

template <class X1,class X2,class X3>
class  QCall3 : public BaseQCall
{
public:
   //---------------------------------------------------------------------------
   // Queued procedure call arguments:

   X1 mX1;
   X2 mX2;
   X3 mX3;

   //---------------------------------------------------------------------------
   // Function call overload. It enqueues a copy of this QCall to the target
   // queue and then notifies the target that a QCall is available.

   void operator()(X1 aX1,X2 aX2,X3 aX3)
   {
      // Store arguments into this QCall.
      mX1=aX1;
      mX2=aX2;
      mX3=aX3;
      // Start a write to the target queue, obtaining memory for a new QCall.
      int tIndex;
      void* tBlock = mTarget->mCallQueue.startWrite(&tIndex);
      if (tBlock==0) return;
      // Create a copy of this QCall, using the new memory.
      QCall3* tQCall = new(tBlock)QCall3(*this);
      // Finish the write to the target queue.
      mTarget->mCallQueue.finishWrite(tIndex);
      // Notify the target.
      mTarget->notifyQCallAvailable();
   }

   //---------------------------------------------------------------------------
   // Execute, it is called by the target thread to execute the QCall. 

   // Execute CallPointer, it contains the address of the function to be called.
   // This typically contains the address of a target class member function.
   typedef Ris::CallPointer3<X1,X2,X3> ExecuteCallPointer;
   ExecuteCallPointer mExecuteCallPointer;

   // This is called by the target thread, after it dequeues the QCall.
   void execute()
   {
      mExecuteCallPointer(mX1,mX2,mX3);
   }

   //---------------------------------------------------------------------------
   // Bind a target to the QCall.

   template <class TargetObject,class CallMethod>
   void bind(TargetObject aTargetObject,CallMethod aCallMethod)
   {
      mTarget = aTargetObject;
      mExecuteCallPointer.bind (aTargetObject,aCallMethod);
   }

   template <class TargetObject,class CallObject,class CallMethod>
   void bind(TargetObject aTargetObject,CallObject aCallObject,CallMethod aCallMethod)
   {
      mTarget = aTargetObject;
      mExecuteCallPointer.bind (aCallObject,aCallMethod);
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// QCall4 is a class template for a QCall with 4 arguments

template <class X1,class X2,class X3,class X4>
class  QCall4 : public BaseQCall
{
public:
   //---------------------------------------------------------------------------
   // Queued procedure call arguments:

   X1 mX1;
   X2 mX2;
   X3 mX3;
   X4 mX4;

   //---------------------------------------------------------------------------
   // Function call overload. It enqueues a copy of this QCall to the target
   // queue and then notifies the target that a QCall is available.

   void operator()(X1 aX1,X2 aX2,X3 aX3,X4 aX4)
   {
      // Store arguments into this QCall.
      mX1=aX1;
      mX2=aX2;
      mX3=aX3;
      mX4=aX4;
      // Start a write to the target queue, obtaining memory for a new QCall.
      int tIndex;
      void* tBlock = mTarget->mCallQueue.startWrite(&tIndex);
      if (tBlock==0) return;
      // Create a copy of this QCall, using the new memory.
      QCall4* tQCall = new(tBlock)QCall4(*this);
      // Finish the write to the target queue.
      mTarget->mCallQueue.finishWrite(tIndex);
      // Notify the target.
      mTarget->notifyQCallAvailable();
   }

   //---------------------------------------------------------------------------
   // Execute, it is called by the target thread to execute the QCall. 

   // Execute CallPointer, it contains the address of the function to be called.
   // This typically contains the address of a target class member function.
   typedef Ris::CallPointer4<X1,X2,X3,X4> ExecuteCallPointer;
   ExecuteCallPointer mExecuteCallPointer;

   // This is called by the target thread, after it dequeues the QCall.
   void execute()
   {
      mExecuteCallPointer(mX1,mX2,mX3,mX4);
   }

   //---------------------------------------------------------------------------
   // Bind a target to the QCall.

   template <class TargetObject,class CallMethod>
   void bind(TargetObject aTargetObject,CallMethod aCallMethod)
   {
      mTarget = aTargetObject;
      mExecuteCallPointer.bind (aTargetObject,aCallMethod);
   }

   template <class TargetObject,class CallObject,class CallMethod>
   void bind(TargetObject aTargetObject,CallObject aCallObject,CallMethod aCallMethod)
   {
      mTarget = aTargetObject;
      mExecuteCallPointer.bind (aCallObject,aCallMethod);
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// QCall5 is a class template for a QCall with 5 arguments

template <class X1,class X2,class X3,class X4,class X5>
class  QCall5 : public BaseQCall
{
public:
   //---------------------------------------------------------------------------
   // Queued procedure call arguments:

   X1 mX1;
   X2 mX2;
   X3 mX3;
   X4 mX4;
   X5 mX5;

   //---------------------------------------------------------------------------
   // Function call overload. It enqueues a copy of this QCall to the target
   // queue and then notifies the target that a QCall is available.

   void operator()(X1 aX1,X2 aX2,X3 aX3,X4 aX4,X5 aX5)
   {
      // Store arguments into this QCall.
      mX1=aX1;
      mX2=aX2;
      mX3=aX3;
      mX4=aX4;
      mX5=aX5;
      // Start a write to the target queue, obtaining memory for a new QCall.
      int tIndex;
      void* tBlock = mTarget->mCallQueue.startWrite(&tIndex);
      if (tBlock==0) return;
      // Create a copy of this QCall, using the new memory.
      QCall5* tQCall = new(tBlock)QCall5(*this);
      // Finish the write to the target queue.
      mTarget->mCallQueue.finishWrite(tIndex);
      // Notify the target.
      mTarget->notifyQCallAvailable();
   }

   //---------------------------------------------------------------------------
   // Execute, it is called by the target thread to execute the QCall. 

   // Execute CallPointer, it contains the address of the function to be called.
   // This typically contains the address of a target class member function.
   typedef Ris::CallPointer5<X1,X2,X3,X4,X5> ExecuteCallPointer;
   ExecuteCallPointer mExecuteCallPointer;

   // This is called by the target thread, after it dequeues the QCall.
   void execute()
   {
      mExecuteCallPointer(mX1,mX2,mX3,mX4,mX5);
   }

   //---------------------------------------------------------------------------
   // Bind a target to the QCall.

   template <class TargetObject,class CallMethod>
   void bind(TargetObject aTargetObject,CallMethod aCallMethod)
   {
      mTarget = aTargetObject;
      mExecuteCallPointer.bind (aTargetObject,aCallMethod);
   }

   template <class TargetObject,class CallObject,class CallMethod>
   void bind(TargetObject aTargetObject,CallObject aCallObject,CallMethod aCallMethod)
   {
      mTarget = aTargetObject;
      mExecuteCallPointer.bind (aCallObject,aCallMethod);
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

