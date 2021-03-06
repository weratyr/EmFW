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

#include "CNaviComponent.h"

CNaviComponent::CNaviComponent(CComponentContext& theContext) :
	AComponentBase(theContext),mGpsDCAdapter(static_cast<CGpsDataContainer&>(CContext::getGpsContext().getContainer()))
{

}

CNaviComponent::~CNaviComponent()
{
}

void CNaviComponent::init(void)
{
	DEBUG_PRINT("entered");
}

void CNaviComponent::run(void)
{
	DEBUG_PRINT("%s start", mContext.getContextNamePtr());
	while (getRun())
	{
		DEBUG_PRINT("%s dispatch loop", mContext.getContextNamePtr());
		mDispatcher.dispatch(true);
	}
}

void CNaviComponent::handleMessage(const CMessage& msg)
{
	DEBUG_PRINT("%s handle message", mContext.getContextNamePtr());
	/*
	 * modified by M. Dederer, S. Eisenhauer MPSE SS 2010
	 * handle GPS messages from new CGpsComponent
	 * initial coding: dump lat/lon to console
	 */
	Component_Index senderIdx = (Component_Index) msg.getSenderID();
	CMessage::MessageType msgType = msg.getMessageType();
	switch (msgType)
	{
		case CMessage::Internal_App_Type: {
			switch(senderIdx) {
				case GPS_INDEX : {
					Int32 ocInt = msg.getOpcode();
					switch(ocInt) {
						case POSITION_DATA: {
							//DEBUG_PRINT("%s received position data: Lat %d.%d Lon %d.%d", mContext.getContextNamePtr(),(msg.getParam1()/1000000),(msg.getParam1()%1000000),(msg.getParam2()/1000000),(msg.getParam2()%1000000));
							DEBUG_PRINT("Sender: %s", CContext::getContext(senderIdx).getContextNamePtr() );
							DEBUG_PRINT("%s received position data: Lat %f Lon %f", mContext.getContextNamePtr(),mGpsDCAdapter.getLatitude(),mGpsDCAdapter.getLongitude());
							DEBUG_PRINT("%s GPS data: state %d, date %02d.%02d.%02d, time %02d:%02d:%02d",
									mContext.getContextNamePtr(),mGpsDCAdapter.getState(),mGpsDCAdapter.getDay(),mGpsDCAdapter.getMonth(),mGpsDCAdapter.getYear()
									,mGpsDCAdapter.getUtcHour(),mGpsDCAdapter.getUtcMinute(),mGpsDCAdapter.getUtcSecond());
							DEBUG_PRINT("%s Sats: visi %d used %d quality %d",mContext.getContextNamePtr(),mGpsDCAdapter.getSatVisi(),mGpsDCAdapter.getSatUsed(),mGpsDCAdapter.getSignalQuality());
							break;
						}
						default:
							break;
					}
					break;
				}
				default:
					break;
			}
			break;
		}
		default: {
			DEBUG_PRINT("%s handling message of type %d not implemented", mContext.getContextNamePtr(),msgType);
			break;
		}
	}
}
