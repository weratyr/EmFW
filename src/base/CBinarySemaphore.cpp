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

#include "CBinarySemaphore.h"

/*
 * constructor. Create a binary semaphore:
 *
 * params: 	isLocked   initial state of the semaphore. false means that the semaphore is empty.
 * 			isProcessShared if true the mutex can be accessed by several processes
 */
CBinarySemaphore::CBinarySemaphore(bool isLocked, bool isProcessShared)
   : mCounter(1)
{
		Int32 retVal;
		// init the pthread_mutexattr_t with default values
		pthread_mutexattr_t mutexAttribut;
		retVal = pthread_mutexattr_init(&mutexAttribut);
		ASSERTION(0 == retVal);
		// for safety we set the values explicitely! needed for pthread_cond_wait
		retVal = pthread_mutexattr_settype(&mutexAttribut, PTHREAD_MUTEX_ERRORCHECK);

		ASSERTION(0 == retVal);
		// set the sharing property
		retVal =  pthread_mutexattr_setpshared(&mutexAttribut,
													isProcessShared ?
													PTHREAD_PROCESS_SHARED
													: PTHREAD_PROCESS_PRIVATE);
		// out commented because retVal is 0 with cygwin
		// ASSERTION(0 == retVal);

		// initialize the mutex with defined structure
		retVal = pthread_mutex_init(&mMutex,&mutexAttribut);
		ASSERTION(0 == retVal);
		// destroy the mutex attribut
		retVal = pthread_mutexattr_destroy(&mutexAttribut);
		ASSERTION(0 == retVal);
		// Attribut for condition variable
		pthread_condattr_t conditionAttribut;
		retVal = pthread_condattr_init(&conditionAttribut);
		ASSERTION(0 == retVal);
		// set the pshared-attribut of the condition variable
		retVal = pthread_condattr_setpshared(&conditionAttribut,
												isProcessShared ?
												PTHREAD_PROCESS_SHARED
												:PTHREAD_PROCESS_PRIVATE);
		// the following assertion is out commented to avoid problem  with cygwin (pthread_condattr_setpshared is not supported)
		// ASSERTION(0 == retVal);
		// init the condition variable. Second parameter is always ignored in Linux
		pthread_cond_init(&mCondition, &conditionAttribut);
		// destroy the attribut
		pthread_condattr_destroy(&conditionAttribut);
		// reset the counter if needed
		if (false == isLocked)
		{
			mCounter = 0;
		}
}

/*
 * destructor
 */
CBinarySemaphore::~CBinarySemaphore()
{
		// destroy the mutex
		pthread_mutex_destroy(&mMutex);
		// destroy the condvar
		Int32 retVal = pthread_cond_destroy(&mCondition);
		retVal=retVal;
		// if a thread still holds the condvar we can not delete it
		ASSERTION(EBUSY != retVal);
}

/*
 * lock the semaphore:
 * If the semaphore is locked by other task, the task will be queued.
 * params: waitForEver if true the task will wait until the semaphore is free.
 * return: true if the lock is successfully done.
 */
bool CBinarySemaphore::take(bool waitForEver)
{
		bool result = true;
		pthread_mutex_lock(&mMutex);
		// free?
		if (mCounter == 1)
		{
			mCounter = 0;
		}
		else if (false == waitForEver)
		{
			result = false;
		}
		else
		{
			// idiom when working with condition variable
			while (0 == mCounter)
			{
				// calling pthread_cond_wait will release mMutex! The calling thread is blocked until the signal is true
				pthread_cond_wait(&mCondition, &mMutex);
			}

			// reset the counter to signal that we have taken it
			mCounter = 0;
		}
		pthread_mutex_unlock(&mMutex);
		return result;
}

/*
 * lock the semaphore:
 * If the semaphore is locked by other task. The task will be queued.
 * params: timeOut is time in milliseconds until task returns. if -1 the task will wait until the semaphore is free forever.
 * return:   true if the lock is successfully done.
 */
bool CBinarySemaphore::takeWithTimeOut(Int32 timeOut)
{
		ASSERTION(-1 <= timeOut);
		// delegate the case -1
		if (-1 == timeOut)
		{
			return take(true);
		}
		else if (0 == timeOut)
		{
			return take(false);
		}

		bool result = true;
		pthread_mutex_lock(&mMutex);
		if (mCounter == 1)
		{
			mCounter = 0;
		}
		else
		{
			// counter is not free
			struct timeval now;
			struct timespec timeExpire;
			// get the current time
			gettimeofday(&now, NULL);
			// calculate the timeout: seconds
			timeExpire.tv_sec = now.tv_sec + (timeOut / 1000);
			// and nano second
			timeExpire.tv_nsec = (now.tv_usec + (timeOut % 1000)*1000) * 1000;

			Int32 retcode = 0;
			while ( (0 == mCounter) && (ETIMEDOUT != retcode) )
			{
				retcode = pthread_cond_timedwait(&mCondition, &mMutex,
											  &timeExpire);
			}
			// check whether the condition is OK or timeout
			result = (ETIMEDOUT != retcode);
			// if mCounter is 1 in the time interval given
			// we must set it to zero again
			if (true == result)
			{
				mCounter = 0;
			}
		}
		pthread_mutex_unlock(&mMutex);
		return result;
			}

/*
 * unlock the semaphore.
*/
void CBinarySemaphore::give(void)
{
		pthread_mutex_lock(&mMutex);
		mCounter = 1;
		// we use signal instead of broadcast because of efficiency
		pthread_mutex_unlock(&mMutex);
		pthread_cond_signal(&mCondition);
}
/***** end of CBinarySemaphore.cpp *****/
