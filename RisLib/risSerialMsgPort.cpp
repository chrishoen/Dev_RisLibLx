/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "my_functions.h"
//#include "risPortableCalls.h"
#include "prnPrint.h"

#include "risSerialMsgPort.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

SerialMsgPort::SerialMsgPort()
{
   mTxMemory = 0;
   mRxMemory = 0;
   mMemorySize = 0;
   mTxMsgCount = 0;
   mRxMsgCount = 0;
   mHeaderAllCount = 0;
   mHeaderOneCount = 0;
   mMonkey = 0;
   mHeaderReadState = 0;
   mHeaderLength = 0;
}

SerialMsgPort::~SerialMsgPort()
{
   if (mTxMemory) free(mTxMemory);
   if (mRxMemory) free(mRxMemory);
   if (mMonkey) delete mMonkey;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize the serial port variables.

void SerialMsgPort::initialize(SerialSettings& aSettings)
{
   // Initialize the base class.
   BaseClass::initialize(aSettings);

   // Create a message monkey.
   mMonkey = BaseClass::mSettings.mMonkeyCreator->createMonkey();

   // Allocate memory for byte buffers.
   mMemorySize = mMonkey->getMaxBufferSize();
   mTxMemory = (char*)malloc(mMemorySize);
   mRxMemory = (char*)malloc(mMemorySize);

   // Set the initial header read state.
   mHeaderReadState = cHeaderReadAll;
   mHeaderLength = mMonkey->getHeaderLength();

   // Initialize the header buffer and allocate memory for it.
   mHeaderBuffer.initialize(mHeaderLength);

   // Initialize variables.
   mTxMsgCount = 0;
   mRxMsgCount = 0;
   mHeaderAllCount = 0;
   mHeaderOneCount = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Copy a message into a byte buffer and then send the byte buffer to the
// serial port with a blocking write call. Return true if successful.
// It is protected by the transmit mutex.

bool SerialMsgPort::doSendMsg(ByteContent* aMsg)
{
   // Guard.
   if (!BaseClass::mValidFlag)
   {
      TS::print(0, "ERROR doSend when Invalid");
      mMonkey->destroyMsg(aMsg);
      return false;
   }

   // Mutex.
   mTxMutex.lock();

   // Create a byte buffer from preallocated memory.
   ByteBuffer tByteBuffer(mTxMemory,mMemorySize);

   // Copy the message to the buffer.
   mMonkey->putMsgToBuffer(&tByteBuffer,aMsg);

   // Delete the message.
   mMonkey->destroyMsg(aMsg);

   // Transmit the buffer.
   int tRet = 0;
   int tLength=tByteBuffer.getLength();
   tRet = BaseClass::doSendBytes(tByteBuffer.getBaseAddress(),tLength);
   TS::print(4, "doSendMsg %d %d",tRet,tLength);

   mTxMsgCount++;

   // Mutex.
   mTxMutex.unlock();

   // Test for errors.
   if (tRet<0)
   {
      TS::print(0, "ERROR SerialMsgPort::doSendMsg FAIL");
      return false;
   }

   // Success.
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive a message from the serial port with a blocking read call into a
// byte buffer and extract a message from the byte buffer. Return the
// message and true if successful. As part of the termination process,
// returning false means that the serial port was closed or that there was
// an error.

bool SerialMsgPort::doReceiveMsg (ByteContent*& aMsg)
{
   TS::print(5, "SerialMsgPort::doReceiveMsg***********************************");

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize a receive byte buffer.

   // Do this first.
   aMsg=0;
   int tRet=0;

   // Guard.
   if (!BaseClass::mValidFlag)
   {
      TS::print(0, "ERROR SerialMsgPort::doReceiveMsg when Invalid");
      return false;
   }

   // Create a byte buffer from preallocated memory.
   ByteBuffer tByteBuffer(mRxMemory, mMemorySize);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read the header from the serial port into the receive byte buffer.
   // Read all header bytes.
   // 
   // This assumes that header synchronization has been attained and that the
   // first byte that is read will be the first byte in a header. If header
   // synchronization has been lost then this will fail and the state will
   // be switched to read the header one byte at a time until a header
   // is detected and synchronization is attained.

   if (mHeaderReadState == cHeaderReadAll)
   {
      // Read the header from the serial port into the byte buffer.
      tRet = BaseClass::doReceiveBytes(tByteBuffer.getBaseAddress(),mHeaderLength);

      // If bad status then return false.
      if (tRet == mHeaderLength)
      {
         TS::print(5, "receive header all %d", tRet);
      }
      else
      {
         TS::print(1, "receive header all ERROR %d", tRet);
         return false;
      }

      // Set the buffer length.
      tByteBuffer.setLength(mHeaderLength);

      // Extract the header parameters from the byte buffer and validate
      // the header.
      mMonkey->extractMessageHeaderParms(&tByteBuffer);

      // If the header is not valid then set the state to read it into 
      // the header buffer one byte at a time.
      if (mMonkey->mHeaderValidFlag)
      {
         TS::print(5, "receive header all PASS");
         mHeaderAllCount++;
      }
      else
      {
         TS::print(1, "receive header all FAIL");
         mHeaderReadState = cHeaderReadOne;
      }
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read the header from the serial port into the serial header buffer.
   // Read the header one byte at a time.
   // 
   // This executes when header synchronization has been lost. It reads one
   // byte at a time and puts the received byte into the header buffer to
   // detect a header and attain synchronization. Once a header has been 
   // detected it sets the next state for synchronized and completes.

   if (mHeaderReadState == cHeaderReadOne)
   {
      // Initialize the header buffer, but do not allocate new memory for it.
      mHeaderBuffer.reinitialize();

      // Loop through the received byte stream to extract the message header.
      bool tGoing = true;
      while (tGoing)
      {
         // Read one byte.
         char tByte;
         tRet = BaseClass::doReceiveOne(&tByte);

         // If bad status then return false.
         if (tRet != 1)
         {
            TS::print(0, "receive header2 ERROR %d", tRet);
            return false;
         }

         // Put it to the header buffer.
         mHeaderBuffer.put(tByte);

         // If the header buffer is full.
         if (mHeaderBuffer.isFull())
         {
            // Copy the header buffer to the beginning of the byte buffer.
            tByteBuffer.setCopyTo();
            tByteBuffer.reset();
            mHeaderBuffer.copyTo(&tByteBuffer);

            // Copy from the byte buffer into the message monkey object,
            // extract the header parameters and validate the header.
            tByteBuffer.setCopyFrom();
            tByteBuffer.rewind();
            mMonkey->extractMessageHeaderParms(&tByteBuffer);

            // If the header is valid then set the header state to read
            // the next header with all of the bytes and exit the loop.
            // If the header is not valid then continue with the loop.
            if (mMonkey->mHeaderValidFlag)
            {
               TS::print(5, "receive header one PASS");
               mHeaderOneCount++;
               mHeaderReadState = cHeaderReadAll;
               tGoing = false;
            }
         }
      }
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read the message payload into the receive buffer.
 
   // Set payload variables.
   int   tPayloadLength = mMonkey->mPayloadLength;
   char* tPayloadBuffer = tByteBuffer.getBaseAddress() + mHeaderLength;

   tRet = BaseClass::doReceiveBytes(tPayloadBuffer,tPayloadLength);

   // If bad status then return false.
   if (tRet == tPayloadLength)
   {
      TS::print(5, "receive payload %d", tRet);
   }
   else
   {
      TS::print(0, "receive payload ERROR %d", tRet);
      return false;
   }

   // Set the buffer length.
   tByteBuffer.setLength(mMonkey->mMessageLength);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // At this point the buffer contains the complete message. Extract the 
   // message from the byte buffer into a new message object and return it.

   // Extract the message from the byte buffer.
   tByteBuffer.rewind();
   aMsg = mMonkey->getMsgFromBuffer(&tByteBuffer);

   // Test for errors.
   if (aMsg==0)
   {
      TS::print(0, "ERROR getMsgFromBuffer");
      return false;
   }

   // Test for message footer errors.
   if (!mMonkey->validateMessageFooter(&tByteBuffer,aMsg))
   {
      TS::print(0, "ERROR validateMessageFooter");
      return false;
   }

   // Done.
   TS::print(4, "SerialMsgPort::doReceiveMsg PASS");
   mRxMsgCount++;
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

}//namespace

