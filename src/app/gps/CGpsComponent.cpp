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

CGpsSystem* CGpsComponent::sGpsSystem =NULL;

CGpsComponent::CGpsComponent(CComponentContext& theContext) :
	AComponentBase(theContext),
	mDevice(GPS_DEVICE),
	mGpsDCAccessor(static_cast<CGpsDataContainer&>(theContext.getContainer()))
{
	mDispatcher.setHandler(*this);

	#ifdef TARGET_D1
		static CGpsSystemQnxSh4 gpsSystem(GPS_DEVICE);
	#else
		static CGpsSystemNMEA gpsSystem(GPS_DEVICE);
	#endif

	sGpsSystem=&gpsSystem;
}

CGpsComponent::~CGpsComponent() {
	stop();
}

void CGpsComponent::init(void) {

	sGpsSystem->init();

	DEBUG_PRINT("entered");
}

void CGpsComponent::run(void) {
	if(!sGpsSystem->isInitialized()) {
		DEBUG_PRINT("GPS not initialized. Exiting...");
		return;
	}
	while (true) {
		if (0 == sGpsSystem->readGpsData(mGpsData) ) {
			this->updateGpsDC();
			this->sendPositionMessage();
		}
	}
}

void CGpsComponent::sendPositionMessage() {
	CMessage msg(CMessage::Internal_App_Type);
	msg.setSenderID(GPS_INDEX);
	msg.setReceiverID(NAVI_INDEX);
	msg.setOpcode(POSITION_DATA);
	CContext::getMDispContext().getNormalQueue().add(msg, false);
}

void CGpsComponent::handleMessage(const CMessage& msg) {

}

void CGpsComponent::updateGpsDC() {
	mGpsDCAccessor.setActualGpsData(mGpsData);
}
