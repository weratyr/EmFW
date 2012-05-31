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

#ifndef _CCOMPONENTCONTEXT_H
#define _CCOMPONENTCONTEXT_H
/**
 * class used to store the necessary data of a context of a component.
 * A component context consists of:
 * - two queues (one for normal and one for system messages. Both queues
 * use a common semaphore that causes blocking when no more message is in
 * the queues.
 * - a semaphore that is used to block both queues and also used to signal
 * that a requested state has been changed or that the watchdog has triggered.
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

#ifndef _FWASSERTION_H
   #include "FWAssertion.h"  		//  for assertion
#endif

#ifndef CCONTAINER_H_
	#include "CContainer.h"
#endif

#include <new>            // for replacement new



//============================================================
// CLASS DEFINITION
//============================================================

class CComponentContext
{
 public:

   /**
    * get the context name
    * @return pointer to a C-string
    */
   const char* getContextNamePtr(void) const;

   /**
    * get the trigger semaphore. It is used by the dispatcher to
    * stay blocking if necessary (no message, no signal)
    * @return  reference to the trigger semaphore
    */
   CBinarySemaphore& getTriggerSemaphore(void);

   /**
    * used for the system communication
    * @return reference to the system queue
    */
   CCommQueue& getSystemQueue(void);


   /**
    * used for the normal communication
    * @return reference to the normal queue
    */
   CCommQueue& getNormalQueue(void);

  /**
    * used for the internal communication
    * @return reference to the normal queue
    */
   CCommQueue& getInternalQueue(void);

   CContainer& getContainer(void);

 private:
   friend class CContext;
   /**
    * create a context for a component. A context is then created
    * ON the memory passed. The memory must be big enough for holding the
    * component context. As a rule the size of the component context is
    * calculated during the compile time!
    * @param contextNamePtr name of the component
    * @param memPtr pointer to the memory area on it the context is created
    * @param systemQueueSize size of the system queue
    * @param normalqueueSize size of the normal queue
    * @param DCSize size of the container
    * @return a context object
    */
   static CComponentContext createComponentContext(Int8* memPtr,
												   const char* contextNamePtr,
                                                   Int32 normalQueueSize,
                                                   Int32 systemQueueSize,
                                                   Int32 internalqueueSize,
                                                   Int32 DCSize,
                                                   void (*createDC)(Int8*)
                                                   );
   /**
    * constructor - private to avoid misuse
    */
   CComponentContext() { };

   /*
    * auskommentiert lassen, da bei aktueller Implementierung benötigt (CContext::createContexts()) und macht kein Ärger im SHM:
    *
    * Kopierkonstruktur liefert zwar ein CComponentContext-Objekt ausserhalb des SHMs, aber die Member-Var-Werte (Pointer auf SHM-Objekte) werden korrekt kopiert;
    *
    * Achtung beim Freibgeben des mittels Kopierkonstruktur erzeugten Objekts: Speicher der Member-Vars werden freigegeben. Dies stellt für unsere Implementierung
    * aber aktuell kein Problem dar, da dieser Speicherbereich im SHM liegt und nicht überschrieben wird
    */
   //   CComponentContext(const CComponentContext& other);
   //   const CComponentContext& operator=(const CComponentContext& other);


   /** hold the context name */
   const char* mContextNamePtr;
   /** pointer to the queues */
   CCommQueue* mNormalQueuePtr;
   CCommQueue* mSystemQueuePtr;
   CCommQueue* mInternalQueuePtr;
   CContainer* mDCPtr;
   /** most data of the context */
   CBinarySemaphore* mTriggerSemaphorePtr;

};

//======================================================================
// INLINE IMPLEMENTATION
//======================================================================

inline const char* CComponentContext::getContextNamePtr(void) const
{
   return mContextNamePtr;
}

inline CBinarySemaphore& CComponentContext::getTriggerSemaphore(void)
{
   return *mTriggerSemaphorePtr;
}

inline CCommQueue& CComponentContext::getNormalQueue(void)
{
   return *mNormalQueuePtr;
}

inline CCommQueue& CComponentContext::getSystemQueue(void)
{
   return *mSystemQueuePtr;
}

inline CCommQueue& CComponentContext::getInternalQueue(void)
{
   return *mInternalQueuePtr;
}


inline CContainer& CComponentContext::getContainer(void)
{
   return *mDCPtr;
}


#endif
