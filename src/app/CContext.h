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

#ifndef _CCONTEXT_H
#define _CCONTEXT_H

//======================================================================
// INCLUDES
//======================================================================
#ifndef _GLOBAL_H
   #include "Global.h"
#endif

#ifndef _CTHREAD_H
   #include "CThread.h"
#endif

#ifndef _CMESSAGE_H
   #include "CMessage.h"
#endif
#ifndef _CCOMMQUEUE_H
   #include "CCommQueue.h"
#endif

#ifndef _CCOMPONENTCONTEXT_H
   #include "CComponentContext.h"
#endif

#ifndef _CTUNERDATACONTAINER_H
   #include "CTunerDataContainer.h"
#endif

#ifndef CGPSDATACONTAINER_H_
   #include "CGpsDataContainer.h"
#endif

#ifndef _FWASSERTION_H
	#include "FWAssertion.h"  //  for assertion
#endif

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
/**
 * these are the Ids for the internal communication between the
 * devices
 */
enum Component_Index
{
	ADMIN_INDEX, /* Admin bekommt eigene Komponente zum Prio setzen */
	MDISP_INDEX,
	HMI_INDEX,
	OpenGL_INDEX,
	TUNER_INDEX,
	NAVI_INDEX,
	GPS_INDEX,
	GPSFILEPARSER_INDEX,
	CD_INDEX,
	INPUT_INDEX,
	/**
	 * Hinweis:
	 * Das hat immer als letzter Eintrag nach den adressierbaren
	 * Komponenten zu folgen!
	 */
	NUM_OF_COMPONENTS
};

	struct CContextDescription						// das stand in CContext.cpp, sollte aber nach CContext.h
													// hier werden fuer jede Komponente die Parameter festgelegt
	{
		Int32 mId;									// id of the component
		const char* mComponentName;					// name of the component
		Int32 mAffinity;							// bound to processor n
		Int32 mStackSize;							// eigentlich nur fuer Threads noetig
		Int32 mPrio;								// CThread::EPriority mPrio; // Prio of the component

		Int32 mNormalQueueSize;
		Int32 mSystemQueueSize;
		Int32 mInternalQueueSize;
		Int32 mDCSize;
		Int32 mWatchdogLimit;						// Initial limit for watchdog

		void (*createDC)(Int8* ptr);

		Int32 mContextSize;							// size needed in shared mem.
	};


//======================================================================
// CLASS DEFINITION
//======================================================================
class CContext
{
 public:
	static CComponentContext& getAdminContext(void);
	static CComponentContext& getMDispContext(void);
	static CComponentContext& getHMIContext(void);
	static CComponentContext& getGLContext(void);
	static CComponentContext& getTunerContext(void);
	static CComponentContext& getNaviContext(void);
	static CComponentContext& getGpsContext(void);
	static CComponentContext& getGpsFileParserContext(void);
	static CComponentContext& getCdContext(void);
	static CComponentContext& getInputContext(void);
	static CComponentContext& getContext(Component_Index Index);

	static void createAdminDC(Int8*);
	static void createMDispDC(Int8*);
	static void createHMIDC(Int8*);
	static void createGLDC(Int8*);
	static void createTunerDC(Int8*);
	static void createNaviDC(Int8*);
	static void createGpsDC(Int8*);
	static void createGpsFileParserDC(Int8*);
	static void createCdDC(Int8*);
	static void createInputDC(Int8*);

	static const Int32 ADMIN_STACK_SIZE;
	static const Int32 MDISP_STACK_SIZE;
	static const Int32 HMI_STACK_SIZE;
	static const Int32 GL_STACK_SIZE;
	static const Int32 TUNER_STACK_SIZE;
	static const Int32 NAVI_STACK_SIZE;
	static const Int32 GPS_STACK_SIZE;
	static const Int32 GPSFILEPARSER_STACK_SIZE;
	static const Int32 CD_STACK_SIZE;
	static const Int32 INPUT_STACK_SIZE;
	static const Int32 DEFAULT_STACK_SIZE;

	static const CThread::EPriority ADMIN_PRIORITY;
	static const CThread::EPriority MDISP_PRIORITY;
	static const CThread::EPriority HMI_PRIORITY;
	static const CThread::EPriority GL_PRIORITY;
	static const CThread::EPriority TUNER_PRIORITY;
	static const CThread::EPriority NAVI_PRIORITY;
	static const CThread::EPriority GPS_PRIORITY;
	static const CThread::EPriority GPSFILEPARSER_PRIORITY;
	static const CThread::EPriority CD_PRIORITY;
	static const CThread::EPriority INPUT_PRIORITY;
	static const CThread::EPriority DEFAULT_PRIORITY;

	static const Int32 ADMIN_AFFINITY;
	static const Int32 MDISP_AFFINITY;
	static const Int32 HMI_AFFINITY;
	static const Int32 GL_AFFINITY;
	static const Int32 TUNER_AFFINITY;
	static const Int32 NAVI_AFFINITY;
	static const Int32 GPS_AFFINITY;
	static const Int32 GPSFILEPARSER_AFFINITY;
	static const Int32 CD_AFFINITY;
	static const Int32 INPUT_AFFINITY;
	static const Int32 DEFAULT_AFFINITY;


 private:
   friend int main(int, char**);
   static void createContexts(void);

   static CComponentContext sContextTable[NUM_OF_COMPONENTS];
};
#endif
