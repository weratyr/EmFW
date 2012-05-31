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

#include "CGpsComponent.h"

CGpsComponent::CGpsComponent(CComponentContext& theContext) :
		AComponentBase(theContext), mGpsDCAccessor(static_cast<CGpsDataContainer&>(CContext::getGpsContext().getContainer()))  {
	mDispatcher.setHandler(*this);
}

CGpsComponent::~CGpsComponent() {
	stop();
}

void CGpsComponent::init(void) {
	DEBUG_PRINT("entered");
}

void CGpsComponent::run(void) {
}

void CGpsComponent::sendPositionMessage() {
	CMessage msg(CMessage::Internal_App_Type);
	msg.setSenderID(GPS_INDEX);
	msg.setReceiverID(HMI_INDEX);
	msg.setOpcode(POSITION_DATA);
	CContext::getMDispContext().getNormalQueue().add(msg, false);
}

void CGpsComponent::handleMessage(const CMessage& msg) {

}
