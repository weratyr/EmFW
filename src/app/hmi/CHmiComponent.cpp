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

#include "CHmiComponent.h"

CHmiComponent::CHmiComponent(CComponentContext& theContext) :
	AComponentBase(theContext)
{

}

CHmiComponent::~CHmiComponent() {
}

void CHmiComponent::init(void) {
	DEBUG_PRINT("entered");
}

void CHmiComponent::run(void)
{
	DEBUG_PRINT("%s starting", mContext.getContextNamePtr());

	//signal first drawing GL-Thread
	CMessage msg;
	CContext::getGLContext().getNormalQueue().add(msg, false);

	while (getRun()) {
		DEBUG_PRINT("%s dispatch loop", mContext.getContextNamePtr());
		mDispatcher.dispatch(true);
	}
}

void CHmiComponent::handleMessage(const CMessage& msg) {
	Int32 mParam;

	mParam = msg.getParam1();
	DEBUG_PRINT("received event with param=%d", mParam);

	CMessage::MessageType messageType = msg.getMessageType();

	switch (messageType) {
		case CMessage::Key_Event_Type:
		{
			//DEBUG_PRINT("Key Event Type switch  - ------ - - - --");
			handleKeyEvent(msg);
			break;
		}
		case CMessage::Internal_App_Type:
		{
			handleKeyEvent(msg);
			break;
		}
		default:
		{
			DEBUG_PRINT(" no match found ");
			return;
		}
	}
	//signal re-drawing
	CMessage msg2;
	CContext::getGLContext().getNormalQueue().add(msg2, false);
}

void CHmiComponent::handleKeyEvent(const CMessage& _m) {
	const IcmKeyCode keycode = (IcmKeyCode) _m.getOpcode(); // could be used for giving the 'x times-pressed' count
	//DEBUG_PRINT("Key Event Type switch  - ------ - - - --");
	switch (keycode) {
	case CD_KEY: {
		DEBUG_PRINT("CD KEY  - ------ - - - --");

		/* HINWEIS:
		 *
		 * Switch the current display by sending a message
		 * to the GL component and setting its parameters in
		 * the DC.
		 */
		CMessage msg(CMessage::Key_Event_Type);
				msg.setSenderID(HMI_INDEX);
				msg.setReceiverID(OpenGL_INDEX);
				msg.setOpcode(CD_KEY);
				CContext::getMDispContext().getNormalQueue().add(msg, false);

	}
		break;

	case TUNER_KEY: {
		DEBUG_PRINT("TUNER KEY  - ------ - - - --");

		/* HINWEIS:
		 *
		 * Switch the current display by sending a message
		 * to the GL component and setting its parameters in
		 * the DC.
		 */
		CMessage msg(CMessage::Key_Event_Type);
				msg.setSenderID(HMI_INDEX);
				msg.setReceiverID(OpenGL_INDEX);
				msg.setOpcode(TUNER_KEY);
				CContext::getMDispContext().getNormalQueue().add(msg, false);
	}
		break;

	default:
		DEBUG_PRINT("UNKNOWN KEY  - ------ - - - --");
		break;
	}
}
