/***************************************************************************
 *  openICM-application
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

#include "CAdminComponent.h"

#define EMPTY_PID -666
pid_t CAdminComponent::mProcessIDs[NUM_OF_COMPONENTS] = { EMPTY_PID };


void CAdminComponent::terminationHandler(int sig)
{
	DEBUG_PRINT("entered for signal %d",sig);
	for (Int32 i = 0; i < NUM_OF_COMPONENTS; i++)
	{
		if ((mProcessIDs[i] != EMPTY_PID) &&  (mProcessIDs[i]!=0))
		{
			DEBUG_PRINT("now kill %s with pid %d",
					CContext::getContext((Component_Index)i).getContextNamePtr(), mProcessIDs[i]);
			kill(mProcessIDs[i], SIGKILL);
			mProcessIDs[i] = EMPTY_PID;
		}
	}
	DEBUG_PRINT("now kill myself (pid=%d)",getpid());
	kill(getpid(), SIGKILL);
}

void setSignalHandler()
{
	DEBUG_PRINT("entered");

	if (signal(SIGTERM, CAdminComponent::terminationHandler) == SIG_IGN)
		signal(SIGTERM, SIG_IGN);
}

CAdminComponent::CAdminComponent(CComponentContext& theContext) :
	AComponentBase(theContext)
{

}

CAdminComponent::~CAdminComponent()
{
}

void CAdminComponent::init(void)
{
	DEBUG_PRINT("entered");

	if ((mProcessIDs[MDISP_INDEX] = fork()) == 0)
	{
		CMainDispatcher mdsp(CContext::getMDispContext());
		CThread CMainDispatcher_thread(mdsp,
				CContext::getMDispContext().getContextNamePtr(),
				CContext::MDISP_STACK_SIZE, CContext::MDISP_PRIORITY,
				CContext::MDISP_AFFINITY, true);
		CMainDispatcher_thread.start();
	}
	else if ((mProcessIDs[HMI_INDEX] = fork()) == 0)
	{
		/*
		 * GL thread for graphics
		 */
		COpenGLComponent glComponent(CContext::getGLContext());
		CThread GLThread(glComponent,
				CContext::getGLContext().getContextNamePtr(),
				CContext::GL_STACK_SIZE,
				CContext::GL_PRIORITY,
				CContext::GL_AFFINITY, false);
		GLThread.start();

		CHmiComponent hmi(CContext::getHMIContext());
		CThread CHmiComponent_thread(hmi,
				CContext::getHMIContext().getContextNamePtr(),
				CContext::HMI_STACK_SIZE, CContext::HMI_PRIORITY,
				CContext::HMI_AFFINITY, true);
		CHmiComponent_thread.start();
	}
	else if ((mProcessIDs[TUNER_INDEX] = fork()) == 0)
	{
		CTunerComponent tuner(CContext::getTunerContext());
		CThread CTunerComponent_thread(tuner,
				CContext::getTunerContext().getContextNamePtr(),
				CContext::TUNER_STACK_SIZE, CContext::TUNER_PRIORITY,
				CContext::TUNER_AFFINITY, true);
		CTunerComponent_thread.start();
	}
	else if ((mProcessIDs[NAVI_INDEX] = fork()) == 0)
	{
		CNaviComponent navi(CContext::getNaviContext());
		CThread CNaviComponent_thread(navi,
				CContext::getNaviContext().getContextNamePtr(),
				CContext::NAVI_STACK_SIZE, CContext::NAVI_PRIORITY,
				CContext::NAVI_AFFINITY, true);
		CNaviComponent_thread.start();
	}
	else if ((mProcessIDs[GPS_INDEX] = fork()) == 0)
	{
		CGpsComponent gps(CContext::getGpsContext());
		CThread CGpsComponent_thread(gps,
				CContext::getGpsContext().getContextNamePtr(),
				CContext::GPS_STACK_SIZE, CContext::GPS_PRIORITY,
				CContext::GPS_AFFINITY, true);
		CGpsComponent_thread.start();
	}

	else if ((mProcessIDs[GPSFILEPARSER_INDEX] = fork()) == 0){
		char pFilename[1024];
		strcpy(pFilename, GPS_CAN_LOGFILE);
		CGpsFileParserComponent gpsfp(CContext::getGpsFileParserContext(), pFilename);
		CThread CGpsFileParserComponent_thread(gpsfp,
				CContext::getGpsFileParserContext().getContextNamePtr(),
				CContext::GPSFILEPARSER_STACK_SIZE, CContext::GPSFILEPARSER_PRIORITY,
				CContext::GPSFILEPARSER_AFFINITY, true);
		CGpsFileParserComponent_thread.start();
	}
	else if ((mProcessIDs[CD_INDEX] = fork()) == 0)
	{
		CCdComponent cd(CContext::getCdContext());
		CThread CCdComponent_thread(cd,
				CContext::getCdContext().getContextNamePtr(),
				CContext::CD_STACK_SIZE, CContext::CD_PRIORITY,
				CContext::CD_AFFINITY, true);
		CCdComponent_thread.start();
	}
	else if ((mProcessIDs[INPUT_INDEX] = fork()) == 0)
	{
		/*
		 * MiniComDevice thread
		 */
		CMiniComDriver miniComDriver;
		CThread miniComDriverThread(miniComDriver, "miniComDriver",
				CContext::DEFAULT_STACK_SIZE, CContext::DEFAULT_PRIORITY,
				CContext::INPUT_AFFINITY, false);
		miniComDriverThread.start();

		/*
		 * Keyboard thread
		 */
		CKeyboardCaptureThread keyboard;
		CThread KeyThread(keyboard, "KeyboardThread",
				CContext::DEFAULT_STACK_SIZE, CContext::DEFAULT_PRIORITY,
				CContext::INPUT_AFFINITY, false);
		KeyThread.start();

		/*
		 * Input-Component as main-thread
		 */
		CInputComponent input(CContext::getInputContext());
		CThread CInputComponent_thread(input,
				CContext::getInputContext().getContextNamePtr(),
				CContext::INPUT_STACK_SIZE, CContext::INPUT_PRIORITY,
				CContext::INPUT_AFFINITY, true);
		CInputComponent_thread.start();
	}

	DEBUG_PRINT("creation of components done");

	setSignalHandler();

	return;
}

void CAdminComponent::run(void)
{
	DEBUG_PRINT("%s run startet", mContext.getContextNamePtr());

	#ifdef AUTO_TERMINATE
		/* Sonder-Implementierung fuer z.B. kurzzeitige Debug- oder Test-Zwecke:
		 *
		 * die Admin dispatcht nicht in einer Endlos-Schleife
		 * sondern nach N Millisekunden alles stoppen
		 *
		 * (AUTO_TERMINATE wird als globales Debug-Flag in Makefile gesetzt bzw. auskommentiert)
		 */
		Int32 seconds_to_wait = 120;
		DEBUG_PRINT("AUTO_TERMINTE-flag is enabled: sleep %d seconds until exit", seconds_to_wait);
		CThread::sleep(seconds_to_wait * 1000);

	#else
		/* regulaere/normale Implementierung der Admin:
		 *
		 * die Admin dispatcht in einer Endlos-Schleife
		 */
		while (getRun()) {
			DEBUG_PRINT("%s dispatch loop", mContext.getContextNamePtr());
			mDispatcher.dispatch(true);
		}
	#endif

	terminationHandler(0);

	return;

}

void CAdminComponent::handleMessage(const CMessage& msg)
{
}
