//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include <signal.h>
#include <poll.h>
#include <sys/eventfd.h>

#include "my_functions.h"
#include "risThreadsThreads.h"
#include "prnPrint.h"

#include "risSerialPort.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Portable specifics.

class SerialPort::Specific
{
public:
   int mPortFd;
   int mEventFd;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

SerialPort::SerialPort()
{
   mSpecific = new Specific;
   mSpecific->mPortFd = 0;
   mSpecific->mEventFd = 0;
}

SerialPort::~SerialPort(void)
{
   doClose();
   delete mSpecific;
}

void SerialPort::initialize(SerialSettings& aSettings)
{
   mSettings = aSettings;
}

bool SerialPort::isValid(){return mValidFlag;}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Open the port.

void SerialPort::doOpen()
{
   mValidFlag=false;

   Prn::print(Prn::SerialInit1,"SerialPort::doOpen %s",mSettings.mPortDevice);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Open the port.

   while (1)
   {
      mSpecific->mPortFd = open(mSettings.mPortDevice, O_RDWR | O_NOCTTY | O_SYNC);

      if (mSpecific->mPortFd < 0)
      {
         Prn::print(Prn::SerialError1, "serial_open_error_1 %d", errno);
         Ris::Threads::threadSleep(2000);
      }
      else
      {
         break;
      }
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Open the port.

   mSpecific->mEventFd = eventfd(0, EFD_SEMAPHORE);

  if (mSpecific->mPortFd < 0)
   {
      Prn::print(Prn::SerialError1, "serial_open_error_2 %d", errno);
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Configure the port for raw data.

   struct termios tOptions;
   tcgetattr(mSpecific->mPortFd, &tOptions);
   tOptions.c_iflag &= ~(INLCR | IGNCR | ICRNL | IXON | IXOFF);
   tOptions.c_oflag &= ~(ONLCR | OCRNL);
   tOptions.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
   tcsetattr(mSpecific->mPortFd, TCSANOW, &tOptions);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Purge.

   Ris::Threads::threadSleep(100);
   doPurge();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Done.
 
   mValidFlag=true;

   Prn::print(Prn::SerialInit2, "SerialMsgPort initialize PASS  $ %s : %16s",
      mSettings.mPortDevice,
      mSettings.mPortSetup);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Close the port.

void SerialPort::doClose()
{
   int tRet = 0;
   if (!mValidFlag) return;

   Prn::print(Prn::SerialInit1, "SerialPort::doClose %s", mSettings.mPortDevice);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Close the port.

   // Write bytes to the event semaphore.
   unsigned long long tCount = 1;
   tRet = write(mSpecific->mEventFd, &tCount, 8);

   // Close the port.
   tRet = close(mSpecific->mPortFd);

   // Test the return code.
   if (tRet != 0)
   {
      Prn::print(Prn::SerialError1, "serial_close_error_2 %d", errno);
   }

   // Done.
   mSpecific->mPortFd = 0;
   mValidFlag = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Purge the port.

void SerialPort::doPurge()
{
   return;
   tcflush(mSpecific->mPortFd, TCIFLUSH);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send data, fixed number of bytes.

int SerialPort::doSendBytes(char* aData, int aNumBytes)
{
   // Guard.
   if (!isValid()) return cRetCodeError;

   // Local variables.
   int tNumWritten = 0;
   int tRet  = 0;

   // Write bytes to the port.
   tRet = write(mSpecific->mPortFd, aData, aNumBytes);

   // Test the return code.
   if (tRet < 0)
   {
      Prn::print(Prn::SerialError1, "serial_write_error_1 %d", errno);
      return cRetCodeError;
   }

   if (tRet != aNumBytes)
   {
      Prn::print(Prn::SerialError1, "serial_write_error_2 %d", tRet);
      return cRetCodeError;
   }

   // Write was successful. Return the number of bytes written.
   Prn::print(Prn::SerialRun4, "SerialPort::doSendBytes PASS1 %d",aNumBytes);
   return tRet;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send data, null terminated string.

int SerialPort::doSendZString(char* aData)
{
   int tNumBytes = (int)strlen(aData);
   return doSendBytes(aData,tNumBytes);   
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send data, one byte.

int SerialPort::doSendOne(char aData)
{
   return doSendBytes(&aData,1);   
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive until eond of line.

int SerialPort::doReceiveUntilEOL(char *aData, int aMaxNumBytes)
{
   int  tStatus = 0;
   int  tIndex = 0;
   int  tRxStatus = 0;
   char tRxChar = 0;
   bool tGoing = true;

   aData[0] = 0;

   // Loop to read single bytes, store them, and exit
   // when termination cr/lf is detected
   while (isValid() && tGoing)
   {
      // Read one byte
      tRxStatus = doReceiveOne(&tRxChar);
      if (tRxStatus >= 0)
      {
         // Read success
         // Store byte
         aData[tIndex] = tRxChar;

         // If CR
         if (tRxChar == 10)
         {
            // Terminator detected, strip if off
            tGoing = false;
            aData[tIndex] = 0;
            tStatus = tIndex;
         }
         if (tIndex == aMaxNumBytes - 1)
         {
            // NumBytes limit was reached
            tGoing = false;
            aData[tIndex] = 0;
            tStatus = tIndex;
         }

         // Increment
         tIndex++;
      }
      else
      {
         // Read failure
         tStatus = tRxStatus;
         tGoing = false;
      }
   }
   return tStatus;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive one byte.

int  SerialPort::doReceiveOne(char *aData)
{
   return doReceiveBytes(aData, 1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive bytes.

int SerialPort::doReceiveBytes(char *aData, int aNumBytes)
{
   Prn::print(Prn::SerialRun3, "SerialPort::doReceiveBytes START %d", aNumBytes);

   // Locals.
   int tRet  = 0;
   
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Set the number of bytes to read.

   struct termios tOptions;
   tcgetattr(mSpecific->mPortFd, &tOptions);
   tOptions.c_cc[VMIN] = aNumBytes;
   tcsetattr(mSpecific->mPortFd, TCSANOW, &tOptions);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Poll the port for a read or a close.

   // Read.
   Prn::print(Prn::SerialRun3, "serial_poll_start");

   struct pollfd tPollFd[2];
   tPollFd[0].fd = mSpecific->mPortFd;
   tPollFd[0].events = POLLIN;
   tPollFd[1].fd = mSpecific->mEventFd;
   tPollFd[1].events = POLLIN;

   // Poll the port for read.
   tRet = poll(&tPollFd[0], 2, -1);

   // Test the return code for error.
   if (tRet < 0)
   {
      Prn::print(Prn::SerialError1, "serial_poll_error_1 %d", errno);
      return cRetCodeError;
   }

   // Test the return code for timeout.
   if (tRet == 0)
   {
      Prn::print(Prn::SerialError1, "serial_poll_error_2 timeout");
      return cRetCodeTimeout;
   }

   // Test the return code for closed port.
   if (tRet == 2)
   {
      Prn::print(Prn::SerialError1, "serial_poll_error_3 close");
      return cRetCodeError;
   }


   Prn::print(Prn::SerialRun3, "serial_poll_pass %d",tRet);


   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read from port.

   // Read.
   Prn::print(Prn::SerialRun3, "serial_read_start");
   tRet = (int)read(mSpecific->mPortFd, aData, (size_t)aNumBytes);

   // Test the return code.
   if (tRet < 0)
   {
      Prn::print(Prn::SerialError1, "serial_read_error_1 %d", errno);
      return cRetCodeError;
   }

   if (tRet != aNumBytes)
   {
      Prn::print(Prn::SerialError1, "serial_read_error_2 %d", tRet);
      return cRetCodeError;
   }

   Prn::print(Prn::SerialRun3, "serial_read_pass");

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Done.

   // Read was successful. Return the number of bytes read.
   Prn::print(Prn::SerialRun3, "SerialPort::doReceiveBytes PASS1 %d", aNumBytes);
   return tRet;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return the number of bytes that are available to receive.

int SerialPort::getAvailableReceiveBytes()
{
   return 99;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

