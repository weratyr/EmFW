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

CommanderKeyCode currentState = MC_SOFT_A_SHORT;

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
	//const IcmKeyCode keycode = (IcmKeyCode) _m.getOpcode(); // could be used for giving the 'x times-pressed' count

	const CommanderKeyCode keycode = (CommanderKeyCode) _m.getOpcode();

	//DEBUG_PRINT("Key Event Type switch  - ------ - - - --");

	if(strlen((char*) _m.getParam4()) > 0)
	{

		//DEBUG_PRINT("KEYCODE: %d", keycode);

		CMessage msg(CMessage::Key_Event_Type);
				msg.setSenderID(HMI_INDEX);
				msg.setReceiverID(OpenGL_INDEX);
				Int8* text = (Int8*) _m.getParam4();
				msg.setParam4(text, strlen((char*) text));
				msg.setOpcode(_m.getOpcode());
				CContext::getMDispContext().getNormalQueue().add(msg, false);





	}
	else
	{
		switch (keycode) {
		case MC_SOFT_A_SHORT: {
			DEBUG_PRINT("MC_SOFT_A_SHORT  - ------ - - - --");

			/* HINWEIS:
			 *
			 * Switch the current display by sending a message
			 * to the GL component and setting its parameters in
			 * the DC.
			 */
			CMessage msg(CMessage::Key_Event_Type);
					msg.setSenderID(HMI_INDEX);
					msg.setReceiverID(TUNER_INDEX);
					msg.setOpcode(MC_SOFT_A_SHORT);
					CContext::getMDispContext().getNormalQueue().add(msg, false);
					currentState = (CommanderKeyCode) msg.getOpcode();

		}
			break;

		case MC_SOFT_B_SHORT: {
			DEBUG_PRINT("MC_SOFT_B_SHORT  - ------ - - - --");

			CMessage msg(CMessage::Key_Event_Type);
					msg.setSenderID(HMI_INDEX);
					msg.setReceiverID(CD_INDEX);
					msg.setOpcode(MC_SOFT_B_SHORT);
					CContext::getMDispContext().getNormalQueue().add(msg, false);
					currentState = (CommanderKeyCode) msg.getOpcode();
		}
			break;

			/*
			 * TUNER 	GRÜN 	A
			 * CD 		ROT 	B
			 */

		case MC_SOFT_ROTARY_RIGHT_UNPRESSED: {

			if(currentState == MC_SOFT_A_SHORT) {
				DEBUG_PRINT("MC_SOFT_ROTARY_RIGHT_UNPRESSED  - ------ - - - --");

				CMessage msg(CMessage::Key_Event_Type);
						msg.setSenderID(HMI_INDEX);
						msg.setReceiverID(TUNER_INDEX);
						msg.setOpcode(MC_SOFT_ROTARY_RIGHT_UNPRESSED);
						CContext::getMDispContext().getNormalQueue().add(msg, false);
			} else if(currentState == MC_SOFT_B_SHORT) {
				CMessage msg(CMessage::Key_Event_Type);
						msg.setSenderID(HMI_INDEX);
						msg.setReceiverID(CD_INDEX);
						msg.setOpcode(MC_SOFT_ROTARY_RIGHT_UNPRESSED);
						CContext::getMDispContext().getNormalQueue().add(msg, false);
			}
		}
				break;
		case MC_SOFT_ROTARY_LEFT_UNPRESSED: {

			if(currentState == MC_SOFT_A_SHORT) {
				DEBUG_PRINT("MC_SOFT_ROTARY_LEFT_UNPRESSED  - ------ - - - --");

				CMessage msg(CMessage::Key_Event_Type);
						msg.setSenderID(HMI_INDEX);
						msg.setReceiverID(TUNER_INDEX);
						msg.setOpcode(MC_SOFT_ROTARY_LEFT_UNPRESSED);
						CContext::getMDispContext().getNormalQueue().add(msg, false);
			} else if(currentState == MC_SOFT_B_SHORT) {
				CMessage msg(CMessage::Key_Event_Type);
						msg.setSenderID(HMI_INDEX);
						msg.setReceiverID(CD_INDEX);
						msg.setOpcode(MC_SOFT_ROTARY_LEFT_UNPRESSED);
						CContext::getMDispContext().getNormalQueue().add(msg, false);
			}
		}
				break;
		default:
			DEBUG_PRINT("UNKNOWN KEY  - ------ - - - --");
			break;
		}
	}
}
