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
 * generic dispatcher that can handle message receiving in a component
 *
 *  class CDispatcher
 *  CDispatcher( CCommQueue& normalQueue, CCommQueue& systemQueue,
 *               CBinarySemaphore& sem);
 *  void setHandler(IMessageHandler& theListener);
 *  void dispatch(bool waitForever = true);
 **************************************************************************/

/**
 * Implementation of the generic dispatcher that dispatch messages to
 * the listeners of the component
 */
//======================================================================
// INCLUDES
//======================================================================
#include "CDispatcher.h"                  // own header file

CDispatcher::CDispatcher( CCommQueue& normalQueue, CCommQueue& systemQueue, CCommQueue& internalQueue,
                        CBinarySemaphore& sem)
						: 	mNormalQueue(normalQueue),
							mSystemQueue(systemQueue),
							mInternalQueue(internalQueue),
							mSem(sem),
							mHandlerPtr(NULLPTR),
							mMessage()
{
}


/**
 * get a command from the queue and dispatch it
 * @param waitForever true => the Dispatcher will be blocked as long,
 * as no command is available
 * handles the watchdog by reseting the context
 */
void CDispatcher::dispatch(bool waitForever)
{
   bool hasMessage = false;
   // as long as the dispatcher is not interrupted it will
   // block if no message is available
   while (false == hasMessage)
   {
      // try to get an system command
      hasMessage = mSystemQueue.getMessage(mMessage);
      if (false == hasMessage)
      {
         hasMessage = mInternalQueue.getMessage(mMessage);
      }
      if (false == hasMessage)
      {
          hasMessage = mNormalQueue.getMessage(mMessage);
      }

      if (true == hasMessage)
      {
         // a command has been found
         break;
      }
      else if (false == waitForever)
      {
         return;
      }
      // at this point the dispatcher must wait for
      // a new message because both queues are empty. mSem is only free
      // if at least one queue is not empty or if a signal is received
      // the context must call interruptDispatcher before

      mSem.take();

      // watchdog trigger might be located here
   }
   if (true == hasMessage)
   {
   	  mHandlerPtr->handleMessage(mMessage);
   }
}
