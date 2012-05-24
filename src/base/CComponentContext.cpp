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

#include "CComponentContext.h"        // own header file


/**
 * create a context for a component. A context is then created
 * ON the memory passed. The memory must be big enough for holding the
 * component context. As a rule the size of the component context is
 * calculated during the compile time!
 * @param contextNamePtr name of the component
 * @param memPtr pointer to the memory area on it the context is created
 * @param systemQueueSize size of the system queue
 * @param normalqueueSize size of the normal queue
 * @return a context object
 */
CComponentContext CComponentContext::createComponentContext(
										  Int8 *memPtr,
										  const char* contextNamePtr,
                                          Int32 normalQueueSize,
                                          Int32 systemQueueSize,
                                          Int32 internalQueueSize,
										  Int32 DCSize,
										  void (*createDC)(Int8*))
{
   CComponentContext ComponentenContext;
   ComponentenContext.mContextNamePtr = contextNamePtr;
   // create data header
   ComponentenContext.mTriggerSemaphorePtr = new(memPtr) CBinarySemaphore(false, true);					// weiter
   memPtr += MAKE_ALIGNMENT_SIZE(sizeof(CBinarySemaphore));

   // and queues
   ComponentenContext.mNormalQueuePtr   = new(memPtr) CCommQueue(normalQueueSize, *ComponentenContext.mTriggerSemaphorePtr);
   memPtr += ComponentenContext.mNormalQueuePtr->getNumOfBytesNeeded();

   ComponentenContext.mSystemQueuePtr   = new(memPtr) CCommQueue(systemQueueSize, *ComponentenContext.mTriggerSemaphorePtr);
   memPtr += ComponentenContext.mSystemQueuePtr->getNumOfBytesNeeded();

   ComponentenContext.mInternalQueuePtr = new(memPtr) CCommQueue(internalQueueSize, *ComponentenContext.mTriggerSemaphorePtr);
   memPtr += ComponentenContext.mInternalQueuePtr->getNumOfBytesNeeded();

   // and DCs (only memory pointer is set - content has to be initialized later @see CContext.cpp)
   ComponentenContext.mDCPtr = reinterpret_cast<CContainer*>(memPtr);
   (*createDC)(memPtr);
   memPtr += DCSize;
   return ComponentenContext;
}

