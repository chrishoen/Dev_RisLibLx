#pragma once

/*==============================================================================
Message serial port class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risByteContent.h"
#include "risByteMsgMonkey.h"
#include "risThreadsSynch.h"
#include "risSerialSettings.h"
#include "risSerialHeaderBuffer.h"
#include "risSerialPort.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Message serial port. This class encapsulates a serial port that
// communicates messages that are based on the byte content message
// encapsulation scheme.
//
// It exchanges byte content messages (send and receive) via a serial port.
//
// It inherits from SerialPort for serial functionality and
// provides methods that can be used to send and receive messages.
//
// Messages are based on the byte content message encapsulation scheme.

class SerialMsgPort : public SerialPort
{
public:
   typedef SerialPort BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants.

   static const int cHeaderReadOne = 1;
   static const int cHeaderReadAll = 2;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Transmit and receive memory. Allocated at initialization.
   char* mTxMemory;
   char* mRxMemory;

   // Size of allocated memory.
   int mMemorySize;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // This a message monkey that is used to manage the details about messages
   // and message headers while hiding the underlying specific message set code.
   // For received  messages, the message monkey allows the receive method to
   // extract message header details from a byte buffer and it allows it to 
   // then extract full messages from the byte buffer. For transmited messages,
   // the message monkey allows the send method to set header data before the
   // message is sent. A specific message monkey is provided by the parent 
   // thread at initialization.
   BaseMsgMonkey* mMonkey;

   // Transmit mutex is used by doSendMsg for mutual exclusion.
   Threads::MutexSemaphore mTxMutex;

   // Metrics.
   int mTxMsgCount;
   int mRxMsgCount;
   int mHeaderAllCount;
   int mHeaderOneCount;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Header read state. This determines if the header is read one byte at 
   // a time and the header buffer is used to detect a header or if it is
   // read all bytes at once.
   int mHeaderReadState;

   // Message header length.
   int mHeaderLength;

   // Serial header buffer. This is used to detect a header and synchronize
   // to it.
   SerialHeaderBuffer mHeaderBuffer;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   SerialMsgPort(); 
  ~SerialMsgPort(); 

   // Initialize and open the serial port.
   void initialize(SerialSettings& aSettings) override;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Copy a message into a byte buffer and then send the byte buffer to the
   // serial port with a blocking write call. Return true if successful.
   // It is protected by the transmit mutex.
   bool doSendMsg(ByteContent*  aTxMsg);

   // Receive a message from the serial port with a blocking read call into a
   // byte buffer and extract a message from the byte buffer. Return the
   // message and true if successful. As part of the termination process,
   // returning false means that the serial port was closed or that there was
   // an error.
   bool doReceiveMsg (ByteContent*& aRxMsg);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

