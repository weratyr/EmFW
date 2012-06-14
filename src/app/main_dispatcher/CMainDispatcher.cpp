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

#include "CMainDispatcher.h"

CMainDispatcher::CMainDispatcher(CComponentContext& theContext) :
	AComponentBase(theContext)
{

}

CMainDispatcher::~CMainDispatcher(void)
{
}

void CMainDispatcher::init(void)
{
	DEBUG_PRINT("entered");
}

/**
 * \brief runs an endless loop to dispatch
 */
void CMainDispatcher::run(void)
{
	DEBUG_PRINT("%s start", mContext.getContextNamePtr());
	while (getRun())
	{
		mDispatcher.dispatch(true);
	}
}

/*
 *  there will arrive some different versions of messages - distinguished by switching the Sendertype
 * - 0x00 (Application message) this message-type is used for communication between the
 * 	  logical devices
 * - 0x01 (Optical MOST) this messages have to be send to the MOST-Bus
 * - 0x02 (Softical MOST) this messages have to be send to internal MOST-Devices - this is done by
 *        the underlying MOST-IO-Layer
 * - 0x03 (received from MOST) - softical or optical - send to the appropriate logical device
 * - 0x04 (key-event messages)
 *
 */
void CMainDispatcher::handleMessage(const CMessage& msg)
{
	DEBUG_PRINT("message type %d", msg.getMessageType());
	// switching the appropriate handle-method
	switch (msg.getMessageType())
	{
	case CMessage::Internal_App_Type:
		handleAppMessage(msg);
		break;
	case CMessage::Optical_Most_To_Type:
	case CMessage::Softical_Most_Type:
		handleMostMessage(msg);
		break;
	case CMessage::Optical_Most_From_Type:
		handleFromMostMessage(msg);
		break;
	case CMessage::Key_Event_Type:
		handleKeyMessage(msg);
		break;
	default:
		DEBUG_PRINT("unknown SenderType. discard message");
		break;
	}

}

/**
 * \brief forwards messages, received from the MOST-Receiver
 *
 * if MOST_SERVER is defined, then there isn't any logical LOCAL device to receive, every message has to be
 * send with the socket-communication to the connected target. this is done in the MOST-IO.
 */
void CMainDispatcher::handleFromMostMessage(const CMessage& msg)
{

}

/**
 * handles application messages. They are used to communicate between the logical devices and are forwarded directly
 * in the addressed queue of the device
 *
 * When the framework is build with the define MOST_SERVER set, the logical devices are nonexistent and so there
 * won't arrive something here
 */
void CMainDispatcher::handleAppMessage(const CMessage& msg)
{
	Int32 id = msg.getReceiverID(); // nur weitersenden
	Component_Index ID = (Component_Index) id;

	CContext::getContext(ID).getNormalQueue().add(msg, false); // send out
}

/**
 * \brief handles messages, which are intended to be sent to the MOST-Bus
 *
 * there is no need for a different activity, if this method is build for a non-MOST-Client (e.g. on Linux),
 * because the MostOverEthernet-Sending-Method is included in the mostIO.
 *
 * So for every message we call CMostIO::send to put the message into the outgoing queue.
 */
void CMainDispatcher::handleMostMessage(const CMessage& msg)
{
}

void CMainDispatcher::handleKeyMessage(const CMessage& msg)
{
	Int32 id = msg.getReceiverID(); // nur weitersenden
	Component_Index ID = (Component_Index) id;

	CContext::getContext(ID).getNormalQueue().add(msg, true); // send out
	//DEBUG_PRINT("Key MSG  : ID %i -- %i   - ------ - - - --", ID, id);
	CContext::getContext(TRACER_INDEX).getNormalQueue().add(msg, true);
}
