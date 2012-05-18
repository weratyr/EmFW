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

//============================================================
// Includes
//============================================================
#include "CThread.h"        // own header file

#include <string.h>

//============================================================
// declaration of static variables of the class CThread
//============================================================
VoidPtrVector  CThread::sThreads(5,5);
CMutex         CThread::sMutex;
pthread_key_t  CThread::sThreadKey;
pthread_once_t CThread::sInit = PTHREAD_ONCE_INIT;
Int32          CThread::sIndex = 0;


unsigned    num_elements = 0;
int         *rsizep, masksize_bytes, size;
int    *rmaskp, *imaskp;
void        *my_data;

//============================================================
// Implementation
//============================================================

/**
 * method that will called only once
 */
void init(void)
{
   pthread_key_create(&CThread::sThreadKey, NULLPTR);
}


/**
 * set affinity for thread to bind on designated CPU
 */
void bindToCPU(Int32 cpu, const char* name)
{
	#ifdef __QNX__

	/* Determine the number of array elements required to hold
	 * the runmasks, based on the number of CPUs in the system. */
	num_elements = RMSK_SIZE(_syspage_ptr->num_cpu);

	/* Determine the size of the runmask, in bytes. */
	masksize_bytes = num_elements * sizeof(unsigned);

	/* Allocate memory for the data structure that we'll pass
	 * to ThreadCtl(). We need space for an integer (the number
	 * of elements in each mask array) and the two masks
	 * (runmask and inherit mask). */

	size = sizeof(int) + 2 * masksize_bytes;
	if ((my_data = malloc(size)) == NULL) {
	    /* Not enough memory. */

	} else {
	    memset(my_data, 0x00, size);

	    /* Set up pointers to the "members" of the structure. */
	    rsizep = (int *)my_data;
	    rmaskp = rsizep + 1;
	    imaskp = rmaskp + num_elements;

	    /* Set the size. */
	    *rsizep = num_elements;

	    /* Set the runmask. Call this macro once for each processor
	       the thread can run on. */
	    RMSK_SET(cpu, rmaskp);

	    /* Set the inherit mask. Call this macro once for each processor
	       the thread's children can run on. */
	    RMSK_SET(cpu, imaskp);


	if ( ThreadCtl( _NTO_TCTL_RUNMASK_GET_AND_SET_INHERIT,
	                   my_data) == -1) {
	        DEBUG_PRINT("Thread Control Start-Error ---------------");
	    }
	}

		//TODO use fuction ThreadCtl( int cmd, void * data ) to control the thread with a runmask, see QNX library reference
		DEBUG_PRINT("WARNING: bind thread [%s] with pid=%d to cpu=%d failed - not implemented yet in qnx ",name, getpid(), cpu);

	#elif __linux__
		//TODO use struct cpu_set_t, for documentation visit http://www.ibm.com/developerworks/linux/library/l-affinity.html
		DEBUG_PRINT("WARNING: bind thread [%s] with pid=%d to cpu=%d failed - not implemented yet in qnx ",name, getpid(), cpu);
	#endif
}

/**
 * Thread start method
 */
void* threadProc(void* threadPtr)
{
   pthread_once(&CThread::sInit, init);
   if (NULLPTR != threadPtr)
   {
      // set the thread-local for fast calling the get-methods
      CThread *thisThreadPtr = reinterpret_cast<CThread*>(threadPtr);
      pthread_setspecific(CThread::sThreadKey ,
                          reinterpret_cast<void *> (thisThreadPtr));
      // bind to cpu
      bindToCPU(thisThreadPtr->mAffinity, thisThreadPtr->mName);
      // call the working method of the thread
      thisThreadPtr->run();
   }
   return NULLPTR;
}

/**
 * Constructor
 * @param runnableObj object that should be run in a thread
 * @param name name of the thread
 * @param staclSize size of the stack
 * @param prio thread priority
 * @param cpu/thread affinity
 * @param asMainThread whether thread is explicitly created or handled as main thread in this context
 */
CThread::CThread(IRunnable &  runnableObj, const char * name,
                 Int32	stackSize,
                 EPriority	prio,
                 Int32 Affinity,
                 bool asMainThread )
   : mRunnablePtr(&runnableObj),
     mName(name),
     mStackSize(stackSize),
     mPrio(prio),
     mAffinity(Affinity),
     mAsMainThread(asMainThread)
{
   ASSERTION(stackSize > 0);
   ASSERTION(mName != NULLPTR);

   // add this thread into the list
   sMutex.take();
   sThreads.add(this);
   sMutex.give();
}

/*
 * destructor
 */
CThread::~CThread()
{
   sMutex.take();
   sThreads.removeElement(this);
   sMutex.give();
}

/**
 * start thread
 */
void CThread::start(void)
{
	int ret; // int because the return type of pthread_create
   struct sched_param threadparam;

   mPID=getpid();

   if (mAsMainThread == false)										// als Thread gestartet
   {
	   // Setting thread attributes
	   // Set the structure to default
	   pthread_attr_t attribute;
	   pthread_attr_init(&attribute);

		// set stack size, scheduling type and priority
	   pthread_attr_setinheritsched(&attribute, PTHREAD_EXPLICIT_SCHED);
	   pthread_attr_setschedpolicy(&attribute, SCHED_RR);
	   pthread_attr_setstacksize(&attribute, mStackSize);
	   pthread_attr_getschedparam(&attribute, &threadparam);
	   threadparam.sched_priority = mPrio;
	   pthread_attr_setschedparam(&attribute, &threadparam);
		ret = pthread_create(&mOSThreadID, &attribute, threadProc, this);

#ifdef __QNX__
		ASSERTION (ret == 0 );
#else
		switch(ret) {
		case 0: break; // no problems :-)
		case 1:
			// The realtime scheduling policies SCHED_RR and SCHED_FIFO
			// are available only to processes with superuser privileges.
			fprintf(stderr, "WARNING: thread creation of [%s] failed due to insufficient permissions - now start with PTHREAD_INHERIT_SCHED\n",mName);
			pthread_attr_setinheritsched(&attribute, PTHREAD_INHERIT_SCHED);
	   ret = pthread_create(&mOSThreadID, &attribute, threadProc, this);
			ASSERTION (ret == 0 );
			break;
		default:
		   DEBUG_PRINT("thread creation of [%s] failed with return code %d (%s)",mName, ret, strerror(ret));
			ASSERTION (ret == 0 );
			break;
	   }
#endif

	   // destroy the attribut
	   pthread_attr_destroy(&attribute);
   }
   else
   {
   		// thread object started as main thread in this context

		// set scheduling and priority
   		int policy;
   		mOSThreadID = pthread_self();
		pthread_getschedparam(mOSThreadID, &policy, &threadparam);
   		threadparam.sched_priority = mPrio;
   		policy = SCHED_RR;
		ret = pthread_setschedparam(mOSThreadID, policy, &threadparam);
#ifdef __QNX__
		ASSERTION(ret == 0);
#else
		// please be aware that special permissions to
		// set scheduling parameters are needed
		// for linux -> run as superuser
		if (ret != 0)
		{
			fprintf(stderr, "WARNING : failed to set scheduling parameter for [%s] with process id %i\n", mName, mPID);
		}
#endif

   		// set main thread's affinity
   		//...
		// bind to cpu
		bindToCPU(mAffinity, mName);

   		run();
   }
}

/**
 * main method of thread
 */
void CThread::run(void)
{
   mRunnablePtr->init();
   mRunnablePtr->run();
   mRunnablePtr->cleanup();
}

/**
 * Sleep without consuming CPU resources.
 *
 * @param mSecs	The period of time, in milli-seconds, that the
 *			currently active thread should sleep.
 */
void CThread::sleep(Int32 mSecs)
{
	   usleep(mSecs*1000);  // unix sleep in nanos
}

   /**
    * get the process ID
    * @param name name of the thread
    * @return process ID (0 if the corresponding thread does not exists)
    */
Int32 CThread::getPID( const char* name )
{
   Int32 pid= 0;
   sMutex.take();
   Int32 i;
   // go through the list and compare the name
   for (i = 0; i < sThreads.getSize(); i++)
   {
      CThread* threadPtr = reinterpret_cast<CThread *> (sThreads[i]);

      if (0 == strcmp(name, threadPtr->mName))
      {

         pid = threadPtr->mPID;
         break;
      }
   }
   sMutex.give();
   return pid;
}

/**
    * get the process ID
    * @return process ID (0 if the corresponding thread does not exists)
    */
Int32 CThread::getCurrentPID()
{
	CThread* currentPtr= reinterpret_cast<CThread*> (pthread_getspecific(sThreadKey));
	return currentPtr->mPID;
}

/**
 * get the thread ID
 * @param name name of the thread
 * @return ID (0 if the corresponding thread does not exists)
 */
TID CThread::getThreadID( const char * name )
{
   TID tid = 0;
   sMutex.take();
   Int32 i;
   // go through the list and compare the name
   for (i = 0; i < sThreads.getSize(); i++)
   {
      CThread* threadPtr = reinterpret_cast<CThread *> (sThreads[i]);
      if (0 == strcmp(name, threadPtr->mName))
      {

         tid = threadPtr->mOSThreadID;
         break;
      }
   }
   sMutex.give();
   return tid;
}

/**
 * get the thread name
 * @param tid ID of the thread
 * @return name (NULLPTR if the corresponding thread does not exists)
 */
const char*  CThread::getThreadName(TID tid)
{
   const char *ptr = NULLPTR;
   sMutex.take();
   // go through the list and compare the ID
   Int32 i;
   for (i = 0; i < sThreads.getSize(); i++)
   {
      CThread* threadPtr = reinterpret_cast<CThread *> (sThreads[i]);
      if (tid == threadPtr->mOSThreadID)
      {

         ptr = threadPtr->mName;
         break;
      }
   }
   sMutex.give();
   return ptr;

}

const char *CThread::getCurrentThreadName(void)
{
   CThread* currentPtr =
      reinterpret_cast<CThread*> (pthread_getspecific(sThreadKey));
   return currentPtr->mName;
}


/**
 * get the pointer to the runnable object
 * @param tid  thread ID
 * @return pointer to the thread ID
 */
IRunnable * CThread::getRunnablePtr(TID tid)
{
   IRunnable *ptr = NULLPTR;
   sMutex.take();
   Int32 i;
   for (i = 0; i < sThreads.getSize(); i++)
   {
      CThread* threadPtr = reinterpret_cast<CThread *> (sThreads[i]);
      if (tid == threadPtr->mOSThreadID)
      {

         ptr = threadPtr->mRunnablePtr;
         break;
      }
   }
   sMutex.give();
   return ptr;

}


IRunnable * CThread::getCurrentRunnablePtr(void)
{
   CThread* currentPtr
      = reinterpret_cast<CThread*> (pthread_getspecific(sThreadKey));
   return currentPtr->mRunnablePtr;
}


// important methods to traverse the list of threads


/**
 * selects first IRunnable of the list. Note that
 * the global mutex is blocked if at least one runnable
 * exists
 * @return pointer to a runnable (NULLPTR if none exists)
 */
IRunnable* CThread::selectFirst(void)
{
   sIndex = 0;
   if (sThreads.getSize() > 0)
   {
      sMutex.take();
      return (reinterpret_cast<CThread*> (sThreads[0]))->mRunnablePtr;
   }
   return NULLPTR;
}

/**
 * selects next runnable. The global mutex is unlocked
 * if no next runnable is possible
 * @return pointer to a runnable (NULLPTR if none exists)
 */
IRunnable* CThread::selectNext(void)
{
   sIndex++;
   if (sThreads.getSize() > sIndex)
   {
      return (reinterpret_cast<CThread*> (sThreads[sIndex]))->mRunnablePtr;
   }
   else
   {
      sMutex.give();
      return NULLPTR;
   }
}

void CThread::join()
{
   pthread_join(mOSThreadID, NULLPTR);
}
