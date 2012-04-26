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

#ifndef _CBINARYSEMAPHORE_H
#define _CBINARYSEMAPHORE_H

/**
 * Simple wrapper for binary semaphore. A semaphore is used to lock a resource. Unlike a mutex a semaphore
 * is not allowed to be taken recursively. Furthermore a semaphore can be released by other thread.
 * This feature allows using semaphores for signalling a thread. The receiver thread will block on some semaphore
 * <code>
 * while (true )
 * {kdfjdkfjdkfjdkfjkd
 * 	sem.take();  // wait for signal
 *	 // get the data and do some works with it
 *	...
 * }
 *
 * The sender thread (it can be an interrupt handler) can signal the receiver by releasing the semaphore.
 * <code>
 * 	sem.give();
 *</code>
 * An object of class CBinarySemaphore can be taken by different processes. It is also designed such that it can be
 * placed into a shared memory
 */
#ifndef _GLOBAL_H
#include "Global.h"
#endif
#ifndef _FWASSERTION_H
   #include "FWAssertion.h"
#endif

#include <sys/time.h>
#include <errno.h>
#include <pthread.h>                      // semaphore must be simulated

class CBinarySemaphore
{
public:
	/**
	 * \brief constructor. Create a binary semaphore.
	 *
	 * \param isLocked   initial state of the semaphore. false means that  the semaphore is empty.
	 * \param isProcessShared if true the mutex can be accessed by several processes
	 */
	CBinarySemaphore(bool isLocked = false, bool isProcessShared = true);
	~CBinarySemaphore();

	/**
	 * \brief lock the semaphore.
	 * If the semaphore is locked by other task, the task will be queued.
	 * \param waitForEver if true the task will wait until the semmaphore is free.
	 * \return   true if the lock is successfully done.
	 */
	bool take(bool waitForEver = true);

	/**
	 * \brief lock the semaphore.
	 * If the semaphore is locked by other task, The task will be queued
	 * \param timeOut is time in milliseconds until task returns. if -1 the task will wait until the semaphore is free forever.
	 * \return   true if the lock is successfully done.
	 */
	bool takeWithTimeOut(Int32 timeOut);

	/**
	 * \brief unlock the semaphore
	 */
	void give(void);

private:
	// to avoid misuse
	CBinarySemaphore(const CBinarySemaphore& rhs);
	CBinarySemaphore& operator=(const CBinarySemaphore& rhs);

	/**
	 * NOTE that POSIX-sem_t does not support the possibility  of timeout. Therefore we must use the mechanism of
	* condition variable to implement a binary semaphore with the well known semantic mutex for loking
	*/
		pthread_mutex_t mMutex;
		// condition variable used to implement take with timeout
		pthread_cond_t mCondition;
		// counter (0 or 1) - Int32 is used because of efficiency
	Int32 mCounter;
};

#endif // end of _CBINARYSEMAPHORE_H
