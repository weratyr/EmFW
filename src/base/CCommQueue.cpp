/***************************************************************************
 *  openICM-framework
 ***************************************************************************
 *  Copyright 2010 Joachim Wietzke and Manh Tien Tran
 *
 *  Contact and informations:
 *  University of Applied Sciences Darmstadt, Germany
 *  	http://www.fbi.h-da.de/~j.wietzke
 *  or
 *  	http://www.fbi.h-da.de/~openicm
 *
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  	http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
*****************************************************************************/

/**
 * Implementation of CCommQueue
 */
#include "CCommQueue.h"              	 // own header file

/**
 * @memo constructor - create an queue
 *
 * @param queueSize   capacity of the queue
 * @param sem Semaphore to signal that an event arrives
 *  (in an empty queue such that the dispatcher can be waken up)
 */
CCommQueue::CCommQueue(Int32 queueSize, CBinarySemaphore& sem)
   : mSignalSem(sem),
     mSize(queueSize),
     mCurrentSize(0),
     mHeadIndex(0),
     mTailIndex(0),
     mAccessSem(true, true),			// free and shared
     mCanWriteSem(true, true)   		// writing into an empty queue is OK
{
   ASSERTION(queueSize > 0);
   // now init the queue correctly
   Int8* ptr = reinterpret_cast<Int8 *> (&mQueue[0]);
   new(ptr) CMessage[queueSize];

}

/**
* @memo destructor - the queue is destroyed.
*/
CCommQueue::~CCommQueue()
{
}



/**
 * @memo add a message into the queue
 *
 * @param    msg      reference to a message
 * @param    waitForever true when the sender can wait
 * @return   true if the event is delivered
 */
bool CCommQueue::add(const CMessage& msg, bool waitForever )
{
   // try to get the write access
   if (false == mCanWriteSem.take(waitForever))
   {
      // does not work and the caller does not want to wait
	  DEBUG_PRINT("could not write message into queue: could not get write-access");
      return false;
   }
   // try to block the queue (no reader is allowed)
   if (false == mAccessSem.take(waitForever))
   {
      // someone has the access right
	  DEBUG_PRINT("could not write message into queue: could not set read-deny");
      mCanWriteSem.give();
      return false;
   }
   // at this point we have the whole control
   mQueue[mTailIndex] = msg;
   mTailIndex--;
   // ring-buffer !!!
   if (mTailIndex < 0)
   {
      mTailIndex = mSize - 1;
   }
   mCurrentSize++;
   // if is the first event we must signal the waiter
   if (1 == mCurrentSize)
   {
      // and signal such that waiting threads can be try again
      mSignalSem.give();
   }


   //give write access if queue is not full
   if (mCurrentSize < mSize)
   {
      mCanWriteSem.give();
   }

   // Give the access right back
   mAccessSem.give();
   return true;
}



/**
 * @memo get a message from the queue. The caller cannot be blocked
 * @param msg object at front of the queue
 * @return true if a message exists, false else
 */
bool CCommQueue::getMessage(CMessage& msg)
{
   // try to access the queue
   mAccessSem.take();
   if (mCurrentSize == 0)
   {
      mAccessSem.give();
      return false;
   }

   // at this point the caller has the whole control about the queue
   msg = mQueue[mHeadIndex];
   mCurrentSize--;
   mHeadIndex--;

   if (mHeadIndex < 0)
   {
      mHeadIndex = mSize - 1;
   }

   // check whether the queue was full. If  not we must allow
   // the write access again
   if ((mSize -1) == mCurrentSize)
   {
      mCanWriteSem.give();
   }
   // do not forget the give the access right back
   mAccessSem.give();
   return true;
}

/**
 * @memo  get the number of events in the queue
 *
 * @return number of events
 */
Int32 CCommQueue::getNumOfMessages(void)
{
   Int32 size;
   mAccessSem.take(true);
   size = mCurrentSize;
   mAccessSem.give();
   return size;
}

// * get the size of the queue

Int32 CCommQueue::getNumOfBytesNeeded(void)
{
   return MAKE_ALIGNMENT_SIZE(sizeof(CCommQueue)
                              + sizeof(CMessage)*(mSize-1));
}


/***** end of CCommQueue.cpp *****/
