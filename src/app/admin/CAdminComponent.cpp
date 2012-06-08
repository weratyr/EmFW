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
		 * TODO
		 * start a OpenGL thread for graphics in HMI process
		 */

		COpenGLComponent opengl(CContext::getGLContext());
				CThread COpenGLComponent_thread(opengl,
						CContext::getGLContext().getContextNamePtr(),
						CContext::GL_STACK_SIZE, CContext::GL_PRIORITY,
						CContext::GL_AFFINITY, false);
				COpenGLComponent_thread.start();


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
//	else if ((mProcessIDs[GPS_INDEX] = fork()) == 0)
//	{
//		CGpsComponent gps(CContext::getGpsContext());
//		CThread CGpsComponent_thread(gps,
//				CContext::getGpsContext().getContextNamePtr(),
//				CContext::GPS_STACK_SIZE, CContext::GPS_PRIORITY,
//				CContext::GPS_AFFINITY, true);
//		CGpsComponent_thread.start();
//	}
	else if ((mProcessIDs[CD_INDEX] = fork()) == 0)
	{
		CCdComponent cd(CContext::getCdContext());
		CThread CCdComponent_thread(cd,
				CContext::getCdContext().getContextNamePtr(),
				CContext::CD_STACK_SIZE, CContext::CD_PRIORITY,
				CContext::CD_AFFINITY, true);
		CCdComponent_thread.start();
	} else if((mProcessIDs[INPUT_INDEX] = fork()) == 0)
	{
	/*
	 * TODO
	 * create a input component with all needed input-types
	 */
		//DEBUG_PRINT("INPUTCOMPONENTE WIRD ERZEUGT!");

//		CKeyboardCaptureThread keyboard;
//				CThread KeyboardComponent_thread(keyboard,
//						CContext::getInputContext().getContextNamePtr(),
//						CContext::DEFAULT_STACK_SIZE, CContext::DEFAULT_PRIORITY,
//						CContext::DEFAULT_AFFINITY, false);
//				KeyboardComponent_thread.start();

		CMiniComDriver miniComDriver;
		CThread miniComDriver_thread(miniComDriver,
				CContext::getInputContext().getContextNamePtr(),
				CContext::DEFAULT_STACK_SIZE, CContext::DEFAULT_PRIORITY,
				CContext::DEFAULT_AFFINITY, false);
		miniComDriver_thread.start();

		CInputComponent input(CContext::getInputContext());
		CThread InputComponent_thread(input,
				CContext::getInputContext().getContextNamePtr(),
				CContext::INPUT_STACK_SIZE, CContext::INPUT_PRIORITY,
				CContext::INPUT_AFFINITY, true);
		InputComponent_thread.start();



	} else if ((mProcessIDs[TRACER_INDEX] = fork()) == 0)
	{
		//CTracerComponent tracer;
		CTracerComponent tracer(CContext::getTracerContext());
		CThread CTracerComponent_thread(tracer,
				CContext::getTracerContext().getContextNamePtr(),
				CContext::TRACER_STACK_SIZE, CContext::TRACER_PRIORITY,
				CContext::TRACER_AFFINITY, true);
		CTracerComponent_thread.start();
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
		/* regular/normal implementierung of admin-component
		 * is to dispatch in an endless loop
		 */
		//while (getRun()) {
			//DEBUG_PRINT("%s dispatch loop", mContext.getContextNamePtr());
			//mDispatcher.dispatch(true);*/


			/*
			 * TODO
			 * remove this code and include regular admin-implementation
			 */

			// e.g. switch to tuner panel after 7 seconds (only works if open gl is running)
			/*
			sleep(7);
			CMessage msg(CMessage::Key_Event_Type);
			msg.setSenderID(ADMIN_INDEX);
			msg.setReceiverID(HMI_INDEX);
			msg.setOpcode(TUNER_KEY);
			CContext::getMDispContext().getNormalQueue().add(msg, false); // send out
			sleep(3);

			DEBUG_PRINT("ADMIN: switched to Tuner done................ now Cd!!!!!!!!!!!!!");
			CMessage msg2(CMessage::Key_Event_Type);
			msg2.setSenderID(ADMIN_INDEX);
			msg2.setReceiverID(HMI_INDEX);
			msg2.setOpcode(CD_KEY);
			CContext::getMDispContext().getNormalQueue().add(msg2, false); // send out
			sleep(3);

			CMessage msg3(CMessage::Key_Event_Type);
			msg3.setSenderID(ADMIN_INDEX);
			msg3.setReceiverID(HMI_INDEX);
			msg3.setOpcode(TUNER_KEY);
			CContext::getMDispContext().getNormalQueue().add(msg3, false); // send out
			sleep(3);
			*/
			/*
			 * now send cyclic (3x) dummy-messages to all other components
			 */
//			int cycles = 3;
//			CMessage msg4(CMessage::Internal_App_Type);
//			msg4.setSenderID(ADMIN_INDEX);
//			msg4.setOpcode(ADMIN_TYPE);
//			for (UInt8 i = 0; i < cycles; i++)
//			{
//				for (int compIDX = HMI_INDEX; compIDX < NUM_OF_COMPONENTS; compIDX++)
//				{
//					msg4.setReceiverID(compIDX);
//					CContext::getMDispContext().getNormalQueue().add(msg4, false);
//					CThread::sleep(1000); //milliSek
//				}
//			}
	//	}
	#endif

	sleep(60);

	terminationHandler(0);

	return;

}

void CAdminComponent::handleMessage(const CMessage& msg)
{
}
