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

/**
 * Structure describing the context of a component - it should be
 * automatically created
 */

// this file will be automatically generated

#include "CContext.h"

const char* ADMIN_COMPONENT_NAME = "AdminComponent";
const char* MDISP_COMPONENT_NAME = "DispComponent";
const char* HMI_COMPONENT_NAME = "HmiComponent";
const char* GL_COMPONENT_NAME = "OpenGLComponent";
const char* TUNER_COMPONENT_NAME = "TunerComponent";
const char* NAVI_COMPONENT_NAME = "NaviComponent";
const char* GPS_COMPONENT_NAME = "GpsComponent";
const char* CD_COMPONENT_NAME = "CdComponent";
const char* INPUT_COMPONENT_NAME = "InputComponent";

const Int32 CContext::ADMIN_STACK_SIZE = 1000;
const Int32 CContext::MDISP_STACK_SIZE = 1000;
const Int32 CContext::HMI_STACK_SIZE = 1000;
const Int32 CContext::GL_STACK_SIZE = 10000;
const Int32 CContext::TUNER_STACK_SIZE = 1000;
const Int32 CContext::NAVI_STACK_SIZE = 1000;
const Int32 CContext::GPS_STACK_SIZE = 1000;
const Int32 CContext::CD_STACK_SIZE = 1000;
const Int32 CContext::INPUT_STACK_SIZE = 1000;
const Int32 CContext::DEFAULT_STACK_SIZE = 1000;

const CThread::EPriority CContext::ADMIN_PRIORITY = CThread::PRIORITY_NORM;
const CThread::EPriority CContext::MDISP_PRIORITY = CThread::PRIORITY_NORM;
const CThread::EPriority CContext::HMI_PRIORITY = CThread::PRIORITY_NORM;
const CThread::EPriority CContext::GL_PRIORITY = CThread::PRIORITY_NORM;
const CThread::EPriority CContext::TUNER_PRIORITY = CThread::PRIORITY_NORM;
const CThread::EPriority CContext::NAVI_PRIORITY = CThread::PRIORITY_NORM;
const CThread::EPriority CContext::GPS_PRIORITY = CThread::PRIORITY_NORM;
const CThread::EPriority CContext::CD_PRIORITY = CThread::PRIORITY_NORM;
const CThread::EPriority CContext::INPUT_PRIORITY = CThread::PRIORITY_NORM;
const CThread::EPriority CContext::DEFAULT_PRIORITY = CThread::PRIORITY_NORM;

/*
 *
 * these are the values defining the cpu/thread affinities;
 * please be aware the designated execution target is defined
 * by use of the cpu id (0-X);
 *
 * actually we use only one cpu (single core)
 */
const Int32 CContext::ADMIN_AFFINITY = 0;
const Int32 CContext::MDISP_AFFINITY = 0;
const Int32 CContext::HMI_AFFINITY = 1;
const Int32 CContext::GL_AFFINITY = 1;
const Int32 CContext::TUNER_AFFINITY = 0;
const Int32 CContext::NAVI_AFFINITY = 0;
const Int32 CContext::GPS_AFFINITY = 0;
const Int32 CContext::CD_AFFINITY = 0;
const Int32 CContext::INPUT_AFFINITY = 0;
const Int32 CContext::DEFAULT_AFFINITY = 0;

const Int32 ADMIN_NORMALQUEUESIZE = 100; // Number of Messages
const Int32 MDISP_NORMALQUEUESIZE = 100;
const Int32 HMI_NORMALQUEUESIZE = 100;
const Int32 GL_NORMALQUEUESIZE = 1; 	//only one event to signaling re-draw
const Int32 TUNER_NORMALQUEUESIZE = 100;
const Int32 NAVI_NORMALQUEUESIZE = 100;
const Int32 GPS_NORMALQUEUESIZE = 100;
const Int32 CD_NORMALQUEUESIZE = 100;
const Int32 INPUT_NORMALQUEUESIZE = 100;

const Int32 ADMIN_SYSTEMQUEUESIZE = 100;
const Int32 MDISP_SYSTEMQUEUESIZE = 100;
const Int32 HMI_SYSTEMQUEUESIZE = 100;
const Int32 GL_SYSTEMQUEUESIZE = 1;
const Int32 TUNER_SYSTEMQUEUESIZE = 100;
const Int32 NAVI_SYSTEMQUEUESIZE = 100;
const Int32 GPS_SYSTEMQUEUESIZE = 100;
const Int32 CD_SYSTEMQUEUESIZE = 100;
const Int32 INPUT_SYSTEMQUEUESIZE = 100;

const Int32 ADMIN_INTERNALQUEUESIZE = 4;
const Int32 MDISP_INTERNALQUEUESIZE = 4;
const Int32 HMI_INTERNALQUEUESIZE = 4;
const Int32 GL_INTERNALQUEUESIZE = 1;
const Int32 TUNER_INTERNALQUEUESIZE = 4;
const Int32 NAVI_INTERNALQUEUESIZE = 4;
const Int32 GPS_INTERNALQUEUESIZE = 4;
const Int32 CD_INTERNALQUEUESIZE = 4;
const Int32 INPUT_INTERNALQUEUESIZE = 4;

const Int32 ADMIN_DCSIZE = 0;
const Int32 MDISP_DCSIZE = 0;
const Int32 HMI_DCSIZE = 0; // HMI braucht keinen Container
const Int32 GL_DCSIZE = 0; // TODO prio 1 :: create DC for Gl-Thread contains what to draw
const Int32 TUNER_DCSIZE = MAKE_ALIGNMENT_SIZE(sizeof(CTunerDataContainer)); // in Byte
const Int32 NAVI_DCSIZE = 0; // das wird ersetzt, wenn die Containter eingefuehrt werden
const Int32 GPS_DCSIZE = MAKE_ALIGNMENT_SIZE(sizeof(CGpsDataContainer));
const Int32 CD_DCSIZE = 0;
const Int32 INPUT_DCSIZE = 0;

//watchdog limits
const Int32 ADMIN_WDLIMIT = 2;
const Int32 MDISP_WDLIMIT = 2;
const Int32 HMI_WDLIMIT = 2;
const Int32 GL_WDLIMIT = 2;
const Int32 TUNER_WDLIMIT = 2;
const Int32 NAVI_WDLIMIT = 2;
const Int32 GPS_WDLIMIT = 2;
const Int32 CD_WDLIMIT = 2;
const Int32 INPUT_WDLIMIT = 2;

//  Groessenberechnungen durch den Compiler
#define HEADER_SIZE  (MAKE_ALIGNMENT_SIZE((sizeof(CBinarySemaphore))))

#define QUEUE_SIZE(numOfMsg)   (MAKE_ALIGNMENT_SIZE((sizeof(CCommQueue)\
	+ sizeof(CMessage)*((numOfMsg) - 1))))

enum
{
	ADMIN_CONTEXT_SIZE = HEADER_SIZE + QUEUE_SIZE(ADMIN_NORMALQUEUESIZE)
			+ QUEUE_SIZE(ADMIN_SYSTEMQUEUESIZE)
			+ QUEUE_SIZE(ADMIN_INTERNALQUEUESIZE) + ADMIN_DCSIZE,
	MDISP_CONTEXT_SIZE = HEADER_SIZE + QUEUE_SIZE(MDISP_NORMALQUEUESIZE)
			+ QUEUE_SIZE(MDISP_SYSTEMQUEUESIZE)
			+ QUEUE_SIZE(MDISP_INTERNALQUEUESIZE) + MDISP_DCSIZE,
	HMI_CONTEXT_SIZE = HEADER_SIZE + QUEUE_SIZE(HMI_NORMALQUEUESIZE)
			+ QUEUE_SIZE(HMI_SYSTEMQUEUESIZE)
			+ QUEUE_SIZE(HMI_INTERNALQUEUESIZE) + HMI_DCSIZE,
	GL_CONTEXT_SIZE = HEADER_SIZE + QUEUE_SIZE(GL_NORMALQUEUESIZE)
			+ QUEUE_SIZE(GL_SYSTEMQUEUESIZE) + QUEUE_SIZE(GL_INTERNALQUEUESIZE)
			+ GL_DCSIZE,
	TUNER_CONTEXT_SIZE = HEADER_SIZE + QUEUE_SIZE(TUNER_NORMALQUEUESIZE)
			+ QUEUE_SIZE(TUNER_SYSTEMQUEUESIZE)
			+ QUEUE_SIZE(TUNER_INTERNALQUEUESIZE) + TUNER_DCSIZE,
	NAVI_CONTEXT_SIZE = HEADER_SIZE + QUEUE_SIZE(NAVI_NORMALQUEUESIZE)
			+ QUEUE_SIZE(NAVI_SYSTEMQUEUESIZE)
			+ QUEUE_SIZE(NAVI_INTERNALQUEUESIZE) + NAVI_DCSIZE,
	GPS_CONTEXT_SIZE = HEADER_SIZE + QUEUE_SIZE(GPS_NORMALQUEUESIZE)
			+ QUEUE_SIZE(GPS_SYSTEMQUEUESIZE) + QUEUE_SIZE(GPS_INTERNALQUEUESIZE)
			+ GPS_DCSIZE,
	CD_CONTEXT_SIZE = HEADER_SIZE + QUEUE_SIZE(CD_NORMALQUEUESIZE)
			+ QUEUE_SIZE(CD_SYSTEMQUEUESIZE) + QUEUE_SIZE(CD_INTERNALQUEUESIZE)
			+ CD_DCSIZE,
	INPUT_CONTEXT_SIZE = HEADER_SIZE + QUEUE_SIZE(INPUT_NORMALQUEUESIZE)
			+ QUEUE_SIZE(INPUT_SYSTEMQUEUESIZE) + QUEUE_SIZE(INPUT_INTERNALQUEUESIZE)
			+ INPUT_DCSIZE,

	TOTAL_SIZE = ADMIN_CONTEXT_SIZE + MDISP_CONTEXT_SIZE + HMI_CONTEXT_SIZE
			+ GL_CONTEXT_SIZE + TUNER_CONTEXT_SIZE + NAVI_CONTEXT_SIZE
			+ GPS_CONTEXT_SIZE + CD_CONTEXT_SIZE + INPUT_CONTEXT_SIZE
};

static const CContextDescription sDescriptionTable[] =
{
{ ADMIN_INDEX, ADMIN_COMPONENT_NAME, CContext::ADMIN_AFFINITY,
		CContext::ADMIN_STACK_SIZE, CContext::ADMIN_PRIORITY,
		ADMIN_NORMALQUEUESIZE, ADMIN_SYSTEMQUEUESIZE, ADMIN_INTERNALQUEUESIZE,
		ADMIN_DCSIZE, ADMIN_WDLIMIT, ADMIN_CONTEXT_SIZE },
{ MDISP_INDEX, MDISP_COMPONENT_NAME, CContext::MDISP_AFFINITY,
		CContext::MDISP_STACK_SIZE, CContext::MDISP_PRIORITY,
		MDISP_NORMALQUEUESIZE, MDISP_SYSTEMQUEUESIZE, MDISP_INTERNALQUEUESIZE,
		MDISP_DCSIZE, MDISP_WDLIMIT, MDISP_CONTEXT_SIZE },
{ HMI_INDEX, HMI_COMPONENT_NAME, CContext::HMI_AFFINITY,
		CContext::HMI_STACK_SIZE, CContext::HMI_PRIORITY, HMI_NORMALQUEUESIZE,
		HMI_SYSTEMQUEUESIZE, HMI_INTERNALQUEUESIZE, HMI_DCSIZE, HMI_WDLIMIT,
		HMI_CONTEXT_SIZE },
{ OpenGL_INDEX, GL_COMPONENT_NAME, CContext::GL_AFFINITY, CContext::GL_STACK_SIZE,
		CContext::GL_PRIORITY, GL_NORMALQUEUESIZE, GL_SYSTEMQUEUESIZE,
		GL_INTERNALQUEUESIZE, GL_DCSIZE, GL_WDLIMIT, GL_CONTEXT_SIZE },
{ TUNER_INDEX, TUNER_COMPONENT_NAME, CContext::TUNER_AFFINITY,
		CContext::TUNER_STACK_SIZE, CContext::TUNER_PRIORITY,
		TUNER_NORMALQUEUESIZE, TUNER_SYSTEMQUEUESIZE, TUNER_INTERNALQUEUESIZE,
		TUNER_DCSIZE, TUNER_WDLIMIT, TUNER_CONTEXT_SIZE },
{ NAVI_INDEX, NAVI_COMPONENT_NAME, CContext::NAVI_AFFINITY,
		CContext::NAVI_STACK_SIZE, CContext::NAVI_PRIORITY,
		NAVI_NORMALQUEUESIZE, NAVI_SYSTEMQUEUESIZE, NAVI_INTERNALQUEUESIZE,
		NAVI_DCSIZE, NAVI_WDLIMIT, NAVI_CONTEXT_SIZE },
{ GPS_INDEX, GPS_COMPONENT_NAME, CContext::GPS_AFFINITY, CContext::GPS_STACK_SIZE,
		CContext::GPS_PRIORITY, GPS_NORMALQUEUESIZE, GPS_SYSTEMQUEUESIZE,
		GPS_INTERNALQUEUESIZE, GPS_DCSIZE, GPS_WDLIMIT, GPS_CONTEXT_SIZE },
{ CD_INDEX, CD_COMPONENT_NAME, CContext::CD_AFFINITY, CContext::CD_STACK_SIZE,
		CContext::CD_PRIORITY, CD_NORMALQUEUESIZE, CD_SYSTEMQUEUESIZE,
		CD_INTERNALQUEUESIZE, CD_DCSIZE, CD_WDLIMIT, CD_CONTEXT_SIZE },
{ INPUT_INDEX, INPUT_COMPONENT_NAME, CContext::INPUT_AFFINITY, CContext::INPUT_STACK_SIZE,
				CContext::INPUT_PRIORITY, INPUT_NORMALQUEUESIZE, INPUT_SYSTEMQUEUESIZE,
				INPUT_INTERNALQUEUESIZE, INPUT_DCSIZE, INPUT_WDLIMIT, INPUT_CONTEXT_SIZE }
};

CComponentContext CContext::sContextTable[NUM_OF_COMPONENTS]; // wg. static


void CContext::createContexts(void)
{
	Int32 fd;
	Int8* ptr;
#define FILE_MODE (S_IRUSR| S_IWUSR | S_IRGRP | S_IROTH)
	shm_unlink("MySharedMem"); // to be sure we start new
	fd = shm_open("MySharedMem", O_RDWR | O_CREAT, FILE_MODE); // SHM immer neu anlegen
	ASSERTION(fd != -1);
	ftruncate(fd, TOTAL_SIZE);

	ptr = reinterpret_cast<Int8 *> (mmap(NULL, TOTAL_SIZE, PROT_READ
			| PROT_WRITE, MAP_SHARED, fd, 0));

	for (Int32 i = 0; i < NUM_OF_COMPONENTS; i++)
	{
		// hier das SHM initialisiert/gefuellt und die Pointer errechnet
		sContextTable[i] = CComponentContext::createComponentContext(ptr,
				sDescriptionTable[i].mComponentName,
				sDescriptionTable[i].mNormalQueueSize,
				sDescriptionTable[i].mSystemQueueSize,
				sDescriptionTable[i].mInternalQueueSize,
				sDescriptionTable[i].mDCSize);

		// CComponentContext weiterschalten
		ptr += sDescriptionTable[i].mContextSize;
	}

	// initialize DCs using the position, calculated during creation of the component context
	ptr = reinterpret_cast<Int8 *> (&(CContext::getTunerContext().getContainer()));
	ASSERTION(reinterpret_cast<Int8 *>(new(ptr) CTunerDataContainer()) == ptr);
}

CComponentContext& CContext::getAdminContext(void)
{
	return sContextTable[ADMIN_INDEX];
}

CComponentContext& CContext::getMDispContext(void)
{
	return sContextTable[MDISP_INDEX];
}

CComponentContext& CContext::getHMIContext(void)
{
	return sContextTable[HMI_INDEX];
}

CComponentContext& CContext::getGLContext(void)
{
	return sContextTable[OpenGL_INDEX];
}

CComponentContext& CContext::getTunerContext(void)
{
	return sContextTable[TUNER_INDEX];
}

CComponentContext& CContext::getNaviContext(void)
{
	return sContextTable[NAVI_INDEX];
}

CComponentContext& CContext::getGpsContext(void)
{
	return sContextTable[GPS_INDEX];
}

CComponentContext& CContext::getCdContext(void)
{
	return sContextTable[CD_INDEX];
}

CComponentContext& CContext::getInputContext(void)
{
	return sContextTable[INPUT_INDEX];
}

CComponentContext& CContext::getContext(Component_Index Index)
{
	return sContextTable[Index];
}
;
