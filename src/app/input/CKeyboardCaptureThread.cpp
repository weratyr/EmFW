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

#include "CKeyboardCaptureThread.h"

void CKeyboardCaptureThread::run()
{
	DEBUG_PRINT("key-input captured Thread ------");
	while(true)
	{
		// read: "character + ENTER" (blocked reading!)
		int ch = getchar(); // alternatives: fgetc(stdin) or getc(stdin)

		switch (ch)
		{
			case 10:
				// ignore return key
			break;
			default:
				// send message to hmi
				DEBUG_PRINT("key-input captured: %i = %c \n", ch, ch);
				CMessage m;
				m.setMessageType(CMessage::Key_Event_Type);
				m.setReceiverID(HMI_INDEX);
				m.setOpcode(ch);
				CContext::getMDispContext().getNormalQueue().add(m, false);
			break;
		}
	}
}
