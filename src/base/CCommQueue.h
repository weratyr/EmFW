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

#ifndef _CCOMMQUEUE_H
#define _CCOMMQUEUE_H
/**
 * CCommQueue-object  is a queue with bounded capacity. It is designed
 * for embedding in a shared memory. Due to the special feature some
 * constraints must be respected
 * <ol>
 * <li> All semaphores can be accessed across processes.
 * <li> To place the queue array (that has a dynamic size) at the end
 *      of the object (in shared memory using a pointer does not work)
 *      an array of "wrong" size 1 is used. The length of the array
 *      is other than 1.
 * <li> to get the size of the object you must use getNumBytesNeeded
 * <\ol>
 * A queue object must not created by normal <code> new </code>.
 * To create a queue you must first get memory (from shared memory or
 * from heap) and then use the replacement new for it.
 * For example if you want to have a queue of size 10 you should write
 * <code>
 * #include <new>   // for replacement
 * ..
 * CBinarySemaphore sem;
 * Int32 sizeNeeded = CCommQueue::getSizeNeeded(10);
 * // get the RAM for it
 * Int8* ptr = new Int8[sizeNeeded];
 * // create the  queue
 * CCommQueue* queuePtr = new(ptr) CCommQueue(10, sem);
 * </code>
 * Note that it is your responsibility to delete the queue
 * correctly. Normally an queue should not be deleted
 */

//============================================================
//   INCLUDES
//============================================================

#ifndef _GLOBAL_H
   #include "Global.h"
#endif
#ifndef _CBINARYSEMAPHORE_H
   #include "CBinarySemaphore.h" // for binary sem
#endif
#ifndef _CMESSAGE_H
	#include "CMessage.h"     //  for sending message
#endif
#ifndef _FWASSERTION_H
    #include "FWAssertion.h"     	     // for assertion
#endif

#include <new>                      	 // for replacement new
//============================================================
// CLASS DEFINITION
//============================================================

class CCommQueue
{

public:

   /**
   * constructor - create an queue
   *
   * @param queueSize   capacity of the queue
   * @param sem Semaphore to signal that an event arrives
   *  (in an empty queue such that the dispatcher can be waken up)
   */
   CCommQueue(Int32 queueSize, CBinarySemaphore& sem);

   /**
   * Destructor
   */
   ~CCommQueue();


   /**
   * add a message into the queue
   *
   * @param    msg      reference to a message
   * @param    waitForever true when the sender can wait
   * @return   true if the event is delivered
   */
   bool add(const CMessage& msg, bool waitForever = true );

   /**
   * @memo  get the number of messages in the queue
   *
   * @return number of messages
   */
   Int32 getNumOfMessages(void);


   /**
    * get a message from the queue. The caller cannot be blocked
    * @param msg object at front of the queue
    * @return true if a message exists, false else
    */
   bool getMessage(CMessage& msg);

   /**
    * get the number of bytes occupied the queue
    * @return memory size occupied by the object
    */
   Int32 getNumOfBytesNeeded(void);

   /**
    * calculate the memory needed to create an  queue with the
    * number of messages needed.
    * @param numOfMsg number of messages
    * @return the number of bytes needed
    */

private:



   // to avoid wrong usage
   CCommQueue           (const CCommQueue& rhs);
   const CCommQueue& operator=(const CCommQueue& rhs);


   CBinarySemaphore    &mSignalSem;        // to signal

   // contains the data
   Int32                mSize;         // size of the queue
   Int32                mCurrentSize;  // current size
   Int32                mHeadIndex;    // first possible element for reading
   Int32                mTailIndex;    // position to insert
   CBinarySemaphore     mAccessSem;    // protect accessing
   CBinarySemaphore     mCanWriteSem;  // used to make client sleeping
   CMessage             mQueue[1];     // dirty trick to force
                                       // the subsequent memory
                                       // belong to the queue. This
                                       // technique is needed for placing
                                       // the queue into a shared memory
}; // end of CCommQueue

#endif // _CCOMMQUEUE_H
