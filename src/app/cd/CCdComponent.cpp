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

#include "CCdComponent.h"

CCdComponent::CCdComponent(CComponentContext& theContext) :
	AComponentBase(theContext)
{

}

CCdComponent::~CCdComponent()
{
}

void CCdComponent::init(void)
{
	DEBUG_PRINT("entered");
}

void CCdComponent::run(void)
{
	DEBUG_PRINT("%s starting", mContext.getContextNamePtr());
	while (getRun())
	{
		DEBUG_PRINT("%s dispatch loop", mContext.getContextNamePtr());
		mDispatcher.dispatch(true);
	}
}

void CCdComponent::handleMessage(const CMessage& msg)
{

	DEBUG_PRINT("%s handle message", mContext.getContextNamePtr());

	CMessage msg2(CMessage::Key_Event_Type);
					msg2.setSenderID(CD_INDEX);
					msg2.setReceiverID(HMI_INDEX);
					msg2.setOpcode(msg.getOpcode());
					Int8* text = (Int8*) "CD Bereit";
					msg2.setParam4(text, strlen((char*) text));
					CContext::getMDispContext().getNormalQueue().add(msg2, false);
}
