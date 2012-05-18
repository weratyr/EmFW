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

#ifndef _CTHREAD_H
#define _CTHREAD_H
/**
 * helper class for easily creating a thread. A thread is the carrier
 * of an IRunnable-object.
 * For easily porting only 16 different priorities are supported. The lowest
 * one is PRIORITY_IDLE and the highest one is PRIORITY_REALTIME.
 *
 * To create a thread you must do the follwing steps:
 * <ol>
 * <li> Write a class that inherit the interface IRunnable.
 * <li> Generate a CThread-object using the constructor of the class
 * <li> call the start method of the thread-object.
 * </ol>
 * The main thread should call join such that the subthreads can be
 * execute its cleanup-method correctly.
 */

//============================================================
// INCLUDES
//============================================================

#ifndef _GLOBAL_H
#include "Global.h"           // global data types
#endif
#ifndef _TVECTOR_H
#include "TVector.h"          // for the list of threads
#endif
#ifndef _CMUTEX_H
#include "CMutex.h"           // to get the list synchronized
#endif
#ifndef _IRUNNABLE_H
#include "IRunnable.h"        // every thread must be an runnable
#endif
#ifndef _FWASSERTION_H
   #include "FWAssertion.h"    // for assertion
#endif

#ifdef __linux__
#include <sched.h>
#endif
#ifdef __QNX__
	#include <sys/neutrino.h>
	#include <sys/syspage.h>
	#include <stdlib.h>
#endif
#include <errno.h>
#include <unistd.h>

//============================================================
// DEFINES
//============================================================
/**
 * TID (Thread ID) is a synonym for pthread_t
 */
#define TID pthread_t

//============================================================
// CLASS DEFINITION
//============================================================



/**
 * Utility class to encapsulate thread  control.
 */
class CThread
{
public:
  /**
   * only 16 priorities are supported
   */
  enum EPriority
     {
        PRIORITY_IDLE = 4,
        PRIORITY_ABOVE_IDLE = 5,
        PRIORITY_BELOW_LOW = 6,
        PRIORITY_LOW = 7,
        PRIORITY_ABOVE_LOW = 8 ,
        PRIORITY_BELOW_NORM = 9,
        PRIORITY_NORM = 10,
        PRIORITY_ABOVE_NORM = 11,
        PRIORITY_BELOW_HIGH = 12,
        PRIORITY_HIGH = 13,
        PRIORITY_ABOVE_HIGH = 14,
        PRIORITY_BELOW_CRITICAL = 15,
        PRIORITY_CRITICAL = 20,
        PRIORITY_ABOVE_CRITICAL = 25,
        PRIORITY_BELOW_REALTIME = 50,
        PRIORITY_REALTIME = 63
     };


   /**
    * Constructor
    * @param runnableObj object that should be run in a thread
    * @param name name of the thread
    * @param staclSize size of the stack
    * @param prio thread priority
    */
   CThread(IRunnable &  runnableObj, const char * name,
           Int32 stackSize, EPriority prio, Int32 Affinity, bool asMainThread = false );
   /**
    * Destructor
    */
   ~CThread();

   /**
    * start the object as a thread
    */
   void start(void);

   /**
    * join the thread
    */
   void join();/*
   {
     pthread_join(mOSThreadID, NULLPTR);
   }*/

  /**
   * Sleep without consuming CPU resources.
   *
   * @param mSecs	The period of time, in milli-seconds, that the
   *			currently active thread should sleep.
   */
   static void sleep(Int32 mSecs);

   /**
    * get the process ID
    * @param name name of the thread
    * @return process ID (0 if the corresponding thread does not exists)
    */
    static Int32 getPID( const char* name );
    static Int32 getCurrentPID();

   /**
    * get the thread ID
    * @param name name of the thread
    * @return ID (0 if the corresponding thread does not exists)
    */
   static TID getThreadID( const char * name );
   static TID getCurrentThreadID(void)
   {
      return pthread_self();
   }


   /**
    * get the thread name
    * @param tid ID of the thread
    * @return name (NULLPTR if the corresponding thread does not exists)
    */
   static const char*  getThreadName(TID tid);
   static const char *getCurrentThreadName(void);


   /**
    * get the pointer to the runnable object
    * @param tid  thread ID
    * @return pointer to the thread ID
    */
   static IRunnable * getRunnablePtr(TID tid);
   static IRunnable * getCurrentRunnablePtr(void);

   // important methods to traverse the list of threads
   /**
    * selects first IRunnable of the list. Note that
    * the global mutex is blocked if at least one runnable
    * exists
    * @return pointer to a runnable (NULLPTR if none exists)
    */
   static IRunnable* selectFirst(void);
   /**
    * selects next runnable. The global mutex is unlocked
    * if no next runnable is possible
    * @return pointer to a runnable (NULLPTR if none exists)
    */
   static IRunnable* selectNext(void);

 private:
  /**
   * C-method to initialise the thread specific data
   */
  friend void init(void);
  /**
   * thread-startmethod
   */
  friend void *threadProc(void *);

   /**
    * Method to activate the thread in its context
    */
   void run();

   IRunnable*           mRunnablePtr;	// object with the thread activity
   const char*          mName;
   Int32                mStackSize;
   EPriority            mPrio;
   Int32				mAffinity;
   bool					mAsMainThread;
   Int32				mPID;
   pthread_t            mOSThreadID;

   // list of all threads
   static VoidPtrVector sThreads;
   // to protect against concurrent accesses
   static CMutex sMutex;
   // used to store the thread-local data
   static pthread_key_t sThreadKey;
   // used to ensure that init-method will be called only once
   static pthread_once_t sInit;
   // used for the traversing the list
   static Int32 sIndex;
};


#endif // end of _CTHREAD_H
