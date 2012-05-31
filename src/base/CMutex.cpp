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

/***********************************************************************
 *		class CMutex
 *		class CMutexGuard
 *		CMutex(bool isFull = true, bool isProcessShared = false);
 *		mutex.take();
 *		mutex.give();
 ***********************************************************************/

/*
 * Implementation of the class CMutex
 */


//============================================================
// INCLUDES
//============================================================
#include "CMutex.h"            // own header file

//============================================================
// IMPLEMENTATION
//============================================================

/**
 * constructor. Create a mutex semaphore.
 *
 * @param isFull   initial state of the semaphore. false means that
 * the semaphore is empty.
 * @param isProcessShared if true the mutex can be accessed by several
 * processes
 */
CMutex::CMutex(bool isFull, bool isProcessShared)
{
   Int32 retVal;
   pthread_mutexattr_t attribut;
   // init the pthread_mutexattr_t with default values
   retVal = pthread_mutexattr_init(&attribut);
   ASSERTION(0 == retVal);

   // to avoid self-blocking we use a recursive mutex.
   // Remember : unlock as often as you locked!
   pthread_mutexattr_settype(&attribut, PTHREAD_MUTEX_RECURSIVE);

   // set the sharing property
   retVal =  pthread_mutexattr_setpshared(&attribut,
					  isProcessShared ?
                                          PTHREAD_PROCESS_SHARED
					  : PTHREAD_PROCESS_PRIVATE);
		ASSERTION(0 == retVal);

   // initialize the mutex with defined structure
   retVal = pthread_mutex_init(&mMutex,&attribut);
   ASSERTION(0 == retVal);
   // initially owned ?
   if (!isFull)
      pthread_mutex_lock(&mMutex);
   // destroy the attribute
   pthread_mutexattr_destroy(&attribut);
}


/**
* destructor.
*/
CMutex::~CMutex()
{
   pthread_mutex_destroy(&mMutex);

}

/**
 * lock the mutex.
 * If the mutex is locked by other task, the task will be queued
 * @param waitForEver if true the task will wait until the
 *        mutex is free.
 * @return true if the lock was successfully done.
 */
bool CMutex::take(bool waitForEver)
{
   // try to lock
   bool haveSem = (0 == pthread_mutex_trylock(&mMutex));

   if ((false == haveSem) && (true == waitForEver))
   {
      // caller will wait
      haveSem = (0 == pthread_mutex_lock(&mMutex));
   }

   return haveSem;
}
/***** end of CMutex.cpp *****/
