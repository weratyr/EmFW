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

#ifndef _IRUNNABLE_H
#define _IRUNNABLE_H


/**
 * Interface for all objects that will be executed in a separate thread
 * or in main thread
 */
class IRunnable
{
 public:
   /**
    * Perform startup actions in thread context.
    * This method will be called precisely once, within the thread context,
    * prior to the call to the 'run' method of this IRBRunnable
    * object. This allows the IRBRunnable to perform any init operations
    * that may be required within the execution context of the thread.
    */
   virtual void init(void) = 0;


   /**
    * Perform the work of this thread
    * This method will be called to perform the work of the IRBRunnable
    * object. This method is not expected to return until either the work
    * of the thread is completed, or until the stop() method of this
    * object has been called.
    */
   virtual void run(void) = 0;


   /**
    * Trigger a shutdown of this thread.
    * The run() method is expected to react to this call by exiting. This
    * method may not be called from within the thread context, for that
    * reason cleanup operations are done separately (see the cleanup method).
    */
   virtual void stop(void) = 0;


   /**
    * Perform cleanup actions within the thread context.
    * This method is called immediately following the exit from the run()
    * method, within the thread context. This is where cleanup actions
    * for this thread should be performed.
    */
   virtual void cleanup(void) = 0;


   /**
    * request to the component to prepare the necessary steps due to
    * possible shutdown
    */
   virtual void prepareForShutdown(void) = 0;

   /**
    * request for resume.
    */
   virtual void reRun() = 0;


   /**
    * request: the component should go to the standby mode
    */
   virtual void goToStandby() = 0;
};

#endif // end of IRUNNABLE_H


