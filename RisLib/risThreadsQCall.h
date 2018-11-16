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

#include <functional>

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

class BaseQCall;

//******************************************************************************
//******************************************************************************
//******************************************************************************
// QCall target base class. Inheriting classes process received QCalls.
// This provides an interface to the qcall classes that hides the details
// of the qcall processing thread.

class BaseQCallTarget
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Try to write a qcall to the to the target queue. Return true if
   // successful.
   virtual bool tryWriteQCall(BaseQCall* aQCall) = 0;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Base qcall class. The different qcalls inherit from this.

class  BaseQCall
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Pointer to the target that the qcall is bound to.
   BaseQCallTarget* mTarget;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   BaseQCall()
   {
      mTarget=0;
   }

   // Return true if this qcall was bound.
   bool isValid(){return mTarget !=0;}

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Execute the qcall. This is called by the target thread.
   virtual void execute() = 0;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// QCall0 is a class template for a qcall with no arguments

class  QCall0 : public BaseQCall
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Execution call pointer, it contains the address of the function to be
   // called. This typically contains the address of a target class member
   // function.
   std::function<void(void)> mExecuteCallPointer;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Invoke the qcall. This is called by a sending thread to send a qcall
   // to the target thread. It enqueues a copy of this qcall to the target
   // queue and then notifies the target thread that a qcall is available.
   void operator()()
   {
      // Create a new copy of the qcall.
      QCall0* tQCall = new QCall0(*this);
      // Try to write the copy to the target queue.
      if (!mTarget->tryWriteQCall(tQCall))
      {
         // The write was not successful.
         delete tQCall;
      }
   }

   // Execute the qcall. This is called by the target thread, after it is
   // notified and dequeues the qcall.
   void execute()
   {
      mExecuteCallPointer();
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Bind a target and call pointer to the qcall.
   template <class TargetObject, class CallMethod>
   void bind(TargetObject aTargetObject, CallMethod aCallMethod)
   {
      using namespace std::placeholders;
      mTarget = aTargetObject;
      mExecuteCallPointer = std::bind(aCallMethod, aTargetObject);
   }

   // Bind a target and call pointer to the qcall.
   template <class TargetObject, class CallObject, class CallMethod>
   void bind(TargetObject aTargetObject, CallObject aCallObject, CallMethod aCallMethod)
   {
      using namespace std::placeholders;
      mTarget = aTargetObject;
      mExecuteCallPointer = std::bind(aCallMethod, aCallObject);
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// QCall1 is a class template for a qcall with 1 argument

template <class X1>
class  QCall1 : public BaseQCall
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Queued procedure call arguments:
   X1 mX1;

   // Execution call pointer, it contains the address of the function to be
   // called. This typically contains the address of a target class member
   // function.
   std::function<void(X1)> mExecuteCallPointer;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Invoke the qcall. This is called by a sending thread to send a qcall
   // to the target thread. It enqueues a copy of this qcall to the target
   // queue and then notifies the target thread that a qcall is available.
   void operator()(X1 aX1)
   {
      // Store arguments for the qcall.
      mX1 = aX1;
      // Create a new copy of the qcall.
      QCall1* tQCall = new QCall1(*this);
      // Try to write the copy to the target queue.
      if (!mTarget->tryWriteQCall(tQCall))
      {
         // The write was not successful.
         delete tQCall;
      }
   }

   // Execute the qcall, passing it the stored argumants. This is called by
   // the target thread, after it dequeues the qcall.
   void execute()
   {
      mExecuteCallPointer(mX1);
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Bind a target and call pointer to the qcall.
   template <class TargetObject, class CallMethod>
   void bind(TargetObject aTargetObject, CallMethod aCallMethod)
   {
      using namespace std::placeholders;
      mTarget = aTargetObject;
      mExecuteCallPointer = std::bind(aCallMethod, aTargetObject, _1);
   }

   // Bind a target and call pointer to the qcall.
   template <class TargetObject, class CallObject, class CallMethod>
   void bind(TargetObject aTargetObject, CallObject aCallObject, CallMethod aCallMethod)
   {
      using namespace std::placeholders;
      mTarget = aTargetObject;
      mExecuteCallPointer = std::bind(aCallMethod, aCallObject, _1);
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// QCall2 is a class template for a qcall with 2 arguments

template <class X1,class X2>
class  QCall2 : public BaseQCall
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Queued procedure call arguments:
   X1 mX1;
   X2 mX2;

   // Execution call pointer, it contains the address of the function to be
   // called. This typically contains the address of a target class member
   // function.
   std::function<void(X1,X2)> mExecuteCallPointer;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Invoke the qcall. This is called by a sending thread to send a qcall
   // to the target thread. It enqueues a copy of this qcall to the target
   // queue and then notifies the target thread that a qcall is available.
   void operator()(X1 aX1,X2 aX2)
   {
      // Store arguments for the qcall.
      mX1 = aX1;
      mX2 = aX2;
      // Create a new copy of the qcall.
      QCall2* tQCall = new QCall2(*this);
      // Try to write the copy to the target queue.
      if (!mTarget->tryWriteQCall(tQCall))
      {
         // The write was not successful.
         delete tQCall;
      }
   }

   // Execute the qcall, passing it the stored argumants. This is called by
   // the target thread, after it dequeues the qcall.
   void execute()
   {
      mExecuteCallPointer(mX1,mX2);
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Bind a target and call pointer to the qcall.
   template <class TargetObject,class CallMethod>
   void bind(TargetObject aTargetObject,CallMethod aCallMethod)
   {
      using namespace std::placeholders;
      mTarget = aTargetObject;
      mExecuteCallPointer = std::bind(aCallMethod, aTargetObject, _1, _2);
   }

   // Bind a target and call pointer to the qcall.
   template <class TargetObject,class CallObject,class CallMethod>
   void bind(TargetObject aTargetObject,CallObject aCallObject,CallMethod aCallMethod)
   {
      using namespace std::placeholders;
      mTarget = aTargetObject;
      mExecuteCallPointer = std::bind(aCallMethod, aCallObject, _1, _2);
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// QCall3 is a class template for a qcall with 3 arguments

template <class X1,class X2,class X3>
class  QCall3 : public BaseQCall
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Queued procedure call arguments:
   X1 mX1;
   X2 mX2;
   X3 mX3;

   // Execution call pointer, it contains the address of the function to be
   // called. This typically contains the address of a target class member
   // function.
   std::function<void(X1,X2,X3)> mExecuteCallPointer;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Invoke the qcall. This is called by a sending thread to send a qcall
   // to the target thread. It enqueues a copy of this qcall to the target
   // queue and then notifies the target thread that a qcall is available.
   void operator()(X1 aX1, X2 aX2, X3 aX3)
   {
      // Store arguments for the qcall.
      mX1 = aX1;
      mX2 = aX2;
      mX3 = aX3;
      // Create a new copy of the qcall.
      QCall3* tQCall = new QCall3(*this);
      // Try to write the copy to the target queue.
      if (!mTarget->tryWriteQCall(tQCall))
      {
         // The write was not successful.
         delete tQCall;
      }
   }

   // Execute the qcall, passing it the stored argumants. This is called by
   // the target thread, after it dequeues the qcall.
   void execute()
   {
      mExecuteCallPointer(mX1,mX2,mX3);
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Bind a target and call pointer to the qcall.
   template <class TargetObject, class CallMethod>
   void bind(TargetObject aTargetObject, CallMethod aCallMethod)
   {
      using namespace std::placeholders;
      mTarget = aTargetObject;
      mExecuteCallPointer = std::bind(aCallMethod, aTargetObject, _1, _2, _3);
   }

   // Bind a target and call pointer to the qcall.
   template <class TargetObject, class CallObject, class CallMethod>
   void bind(TargetObject aTargetObject, CallObject aCallObject, CallMethod aCallMethod)
   {
      using namespace std::placeholders;
      mTarget = aTargetObject;
      mExecuteCallPointer = std::bind(aCallMethod, aCallObject, _1, _2, _3);
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// QCall4 is a class template for a qcall with 4 arguments

template <class X1,class X2,class X3,class X4>
class  QCall4 : public BaseQCall
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Queued procedure call arguments:
   X1 mX1;
   X2 mX2;
   X3 mX3;
   X4 mX4;

   // Execution call pointer, it contains the address of the function to be
   // called. This typically contains the address of a target class member
   // function.
   std::function<void(X1, X2, X3, X4)> mExecuteCallPointer;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Invoke the qcall. This is called by a sending thread to send a qcall
   // to the target thread. It enqueues a copy of this qcall to the target
   // queue and then notifies the target thread that a qcall is available.
   void operator()(X1 aX1, X2 aX2, X3 aX3, X4 aX4)
   {
      // Store arguments for the qcall.
      mX1 = aX1;
      mX2 = aX2;
      mX3 = aX3;
      mX4 = aX4;
      // Create a new copy of the qcall.
      QCall4* tQCall = new QCall4(*this);
      // Try to write the copy to the target queue.
      if (!mTarget->tryWriteQCall(tQCall))
      {
         // The write was not successful.
         delete tQCall;
      }
   }

   // Execute the qcall, passing it the stored argumants. This is called by
   // the target thread, after it dequeues the qcall.
   void execute()
   {
      mExecuteCallPointer(mX1,mX2,mX3,mX4);
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Bind a target and call pointer to the qcall.
   template <class TargetObject, class CallMethod>
   void bind(TargetObject aTargetObject, CallMethod aCallMethod)
   {
      using namespace std::placeholders;
      mTarget = aTargetObject;
      mExecuteCallPointer = std::bind(aCallMethod, aTargetObject, _1, _2, _3, _4);
   }

   // Bind a target and call pointer to the qcall.
   template <class TargetObject, class CallObject, class CallMethod>
   void bind(TargetObject aTargetObject, CallObject aCallObject, CallMethod aCallMethod)
   {
      using namespace std::placeholders;
      mTarget = aTargetObject;
      mExecuteCallPointer = std::bind(aCallMethod, aCallObject, _1, _2, _3, _4);
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// QCall5 is a class template for a qcall with 5 arguments

template <class X1,class X2,class X3,class X4,class X5>
class  QCall5 : public BaseQCall
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Queued procedure call arguments:
   X1 mX1;
   X2 mX2;
   X3 mX3;
   X4 mX4;
   X5 mX5;

   // Execution call pointer, it contains the address of the function to be
   // called. This typically contains the address of a target class member
   // function.
   std::function<void(X1, X2, X3, X4, X5)> mExecuteCallPointer;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Invoke the qcall. This is called by a sending thread to send a qcall
   // to the target thread. It enqueues a copy of this qcall to the target
   // queue and then notifies the target thread that a qcall is available.
   void operator()(X1 aX1, X2 aX2, X3 aX3, X4 aX4,X5 aX5)
   {
      // Store arguments for the qcall.
      mX1 = aX1;
      mX2 = aX2;
      mX3 = aX3;
      mX4 = aX4;
      mX5 = aX5;
      // Create a new copy of the qcall.
      QCall5* tQCall = new QCall5(*this);
      // Try to write the copy to the target queue.
      if (!mTarget->tryWriteQCall(tQCall))
      {
         // The write was not successful.
         delete tQCall;
      }
   }

   // Execute the qcall, passing it the stored argumants. This is called by
   // the target thread, after it dequeues the qcall.
   void execute()
   {
      mExecuteCallPointer(mX1,mX2,mX3,mX4,mX5);
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Bind a target and call pointer to the qcall.
   template <class TargetObject, class CallMethod>
   void bind(TargetObject aTargetObject, CallMethod aCallMethod)
   {
      using namespace std::placeholders;
      mTarget = aTargetObject;
      mExecuteCallPointer = std::bind(aCallMethod, aTargetObject, _1, _2, _3, _4, _5);
   }

   // Bind a target and call pointer to the qcall.
   template <class TargetObject, class CallObject, class CallMethod>
   void bind(TargetObject aTargetObject, CallObject aCallObject, CallMethod aCallMethod)
   {
      using namespace std::placeholders;
      mTarget = aTargetObject;
      mExecuteCallPointer = std::bind(aCallMethod, aCallObject, _1, _2, _3, _4, _5);
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

