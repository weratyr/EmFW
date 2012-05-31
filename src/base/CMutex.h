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

/**
 * This is a simple wrapper class for mutex. A mutex can be taken
 * recursively. Only the thread that currently takes the mutex
 * is allowed to release it. Typically a mutex is used to protect
 * a critical section. In this implementation class mutex can be
 * used across process boundary. It is needed to protect common
 * resource placed in the shared memory.
 * To work with a critical section you normally first lock the mutex,
 * use the resource and finally release it.
 * <code>
 * mutex.take();
 * // do some stuffs
 * ...
 * mutex.give();
 * </code>
 * To simplify using the mutex the helper class CMutexGuard is
 * introduced. You simply write
 * <code>
 * CMutexGuard guard(mutex);
 * // do some stuffs
 * </code>
 * Leaving the scope the destructor of <code>guard</code> is called
 * such that the mutex is released. Using the guard in a function that
 * takes a longtime is dangerous. The mutex is only released at the end
 * of the scope that can be the end of the function.
 */


#ifndef _CMUTEX_H
#define _CMUTEX_H

//============================================================
// INCLUDES
//============================================================

#ifndef _GLOBAL_H
#include "Global.h"                    // for the right configuration
#endif
#ifndef _FWASSERTION_H
   #include "FWAssertion.h"    // for assertion
#endif

#include <pthread.h>                   // POSIX-lib

//============================================================
// CLASS DEFINITION
//============================================================

/**
*  mutual exclusive semaphore.
*/
class CMutex
{
public:

   /**
    * constructor. Create a mutex semaphore.
    *
    * @param isFull   initial state of the semaphore. false means that
    * the semaphore is empty.
    * @param isProcessShared if true the mutex can be accessed by several
    * processes
    */
   CMutex(bool isFull = true, bool isProcessShared = true);


   /**
    * Destructor.
    */
   ~CMutex();


   /**
    * lock the mutex.
    * If the mutex is locked by other task, the task
    *        will be queued
    * @param waitForEver if true the task will wait until the
    *        mutex is free.
    * @return true if the lock was successfully done.
    */
   bool take(bool waitForEver = true);


   /**
    *   free the mutex
    */
   void give(void);

private:
   // to avoid misuse
   CMutex(const CMutex& rhs);
   CMutex& operator=(const CMutex& rhs);
   // mutex
   pthread_mutex_t mMutex;
}; // end of class CMutex


//============================================================
// INLINE-IMPLEMENTATION
//============================================================

/*
 * unlock the mutex
 */
inline void CMutex::give(void)
{
   pthread_mutex_unlock(&mMutex);
}



/**
 * helpe class to autorelease the mutex
 */
class CMutexGuard
{
 public:
   /**
    * constructor
    * @param mutex mutex to guard
    */
   CMutexGuard(CMutex& mutex)
      : mMutexPtr(&mutex), mIsOwner(false)
   {
      take();
   }

   /**
    * destructor
    */
   ~CMutexGuard()
   {
      give();
   }

   void take(void)
   {
      if (mMutexPtr->take())
      {
         mIsOwner = true;
      }
   }

   void give(void)
   {
      if (true == mIsOwner)
      {
         mMutexPtr->give();
      }
   }

 private:
   // to avoid misuse
   CMutexGuard(const CMutexGuard&);
   const CMutexGuard& operator=(const CMutexGuard&);

   // mutex to guard
   CMutex* mMutexPtr;
   // to remember whether the lock is successful
   bool mIsOwner;
};

#endif // end of _CMUTEX_H
