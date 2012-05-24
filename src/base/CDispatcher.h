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

#ifndef _CDISPATCHER_H
#define _CDISPATCHER_H
/**
 * generic dispatcher that can handle message receiving in a component
 */
//======================================================================
// INCLUDES
//======================================================================
#ifndef _GLOBAL_H
   #include "Global.h"
#endif

#ifndef _CBINARYSEMAPHORE_H
   #include "CBinarySemaphore.h" // for binary sem
#endif

#ifndef _CMESSAGE_H
	#include "CMessage.h"     //  for sending message
#endif

#ifndef _CCOMMQUEUE_H
   #include "CCommQueue.h"
#endif

#ifndef _CCOMPONENTCONTEXT_H
   #include "CComponentContext.h"
#endif

#ifndef _FWASSERTION_H
   #include "FWAssertion.h"  		//  for assertion
#endif


//======================================================================
// CLASS DEFINITION
//======================================================================
/**
 * a dispatcher will get commands from the queues and deliver them
 */
class CDispatcher
{
public:

/*
 * constructor
 * @param context context of the component (class CComponentContext)
*/

   CDispatcher( CCommQueue& normalQueue, CCommQueue& systemQueue, CCommQueue& internalQueue,
	                CBinarySemaphore& sem);

   /**
    *  Destructor
    */
   ~CDispatcher();


   /**
    * set the handler
    * @param theListener object handling message received
    */
   void setHandler(IMessageHandler& theListener);


   /**
    * get a command from the queue and dispatch it
    * @param waitForever true => the Dispatcher will be blocked as long,
    * as no command is avaiable
    */
   void dispatch(bool waitForever = true);

private:
   // to avoid misuse
   CDispatcher(const CDispatcher& rhs);
   CDispatcher& operator=(const CDispatcher& rhs);

protected:
	   /**
	    * Queues for external commands
	    */
	   CCommQueue&            mNormalQueue;
	   CCommQueue&            mSystemQueue;
	   CCommQueue&			  mInternalQueue;
	   /**
	    * Semaphore used to unblock the dispatcher
	    */
	   CBinarySemaphore& mSem;
	   IMessageHandler      * mHandlerPtr;

	   /**
	    * to hold the current message
	    */
	   CMessage             mMessage;


};

//======================================================================
// INLINE IMPLEMENTATION
//======================================================================

inline CDispatcher::~CDispatcher()
{
}

inline void CDispatcher::setHandler(IMessageHandler& theListener)
{
   mHandlerPtr = &theListener;
}
#endif // end of _CDISPATCHER_H

