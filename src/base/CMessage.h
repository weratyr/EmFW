/***************************************************************************
 *  openICM-framework
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

/**
 * Simple structure used for message sent between the components.
 * A message consists of
 * <ol>
 * <li> senderID an ID of the sender component. With this ID
 *      an handler in the receiver part is easily identified.
 * <li> receiverID ID of the receiver in the component.
 * <li> opCode action that should be taken (an event!)
 * <li> three parameters.
 * <li> a message handler. Message handler is ignored if it sent
 *      from one thread to an other. While sending a message in the same
 *      thread context the sender can explicitely set the handler.
 *      A correct handler for external message is found using
 *      the local registry.
 * </ol>
 */

#ifndef _CMESSAGE_H
#define _CMESSAGE_H

//============================================================
// INCLUDES
//============================================================

#ifndef _GLOBAL_H
   #include "Global.h"
#endif

#ifndef _IMESSAGEHANDLER_H
	#include "IMessageHandler.h"
#endif

#ifndef _FWASSERTION_H
	#include "FWAssertion.h"
#endif

#include <string.h>     // for memset


//======================================================================
// FORWARD DECLARATIONS
//======================================================================
class IMessageHandler;


struct MostMessage
{

	Int32 mType;					// siehe unten Enums   MOST size = 19 Byte, total 22
	union {
		unsigned char bytes[19];	/**< raw most data */
		struct {
			UInt8 FBlockID;
			UInt8 Instance;
			UInt16 FkID;
			UInt8 OPType;
			UInt16 Length;
			UInt8 data[12];
		} rx_msg;
	} data;


};

struct AppMessage					// total 64 Byte
{
	Int32 mType;
	Int32 senderID;
	Int32 receiverID;
	Int32 opcode;
	Int32 parameter1;
	Int32 parameter2;
	Int32 parameter3;
	Int8 parameter4[32];
	IMessageHandler* mMessageHandlerPtr;	   			// fuer Registries
};


//============================================================
// CLASS DEFINITION
//============================================================


class CMessage
{
 public:

	enum MessageType
 	{
		Key_Event_Type, /* raised by keyboard- and MiniCom-input */
		Internal_App_Type,/* Messages created inside the FW software */
 		Optical_Most_From_Type,
 		Optical_Most_To_Type,
 		Softical_Most_Type,
 	};

   CMessage();
   CMessage(MessageType theMsgType);
   CMessage(MostMessage msg);

   // SET-METHODS
   void setSenderType(UInt8 senderType);
   void setMessageType(MessageType type);
   void setSenderID(Int32 senderID);
   void setReceiverID(Int32 receiverID);
   void setOpcode(Int32 opcode);
   void setParam1(Int32 param);
   void setParam2(Int32 param);
   void setParam3(Int32 param);
   void setParam4(Int8* byteArrayPtr, Int8 len);
   void setMostMessage(MostMessage);
   void setMessageHandlerPtr(IMessageHandler* handlerPtr);


   // GET-METHODS
   UInt8 getSenderType(void) const;
   MessageType getMessageType(void) const;
   Int32 getSenderID(void) const;
   Int32 getReceiverID(void) const;
   Int32 getOpcode(void) const;
   Int32 getParam1(void) const;
   Int32 getParam2(void) const;
   Int32 getParam3(void) const;
   const Int8* getParam4(void) const;
   MostMessage* getRawData();
   MostMessage getStructMostMessage(void) const;
   IMessageHandler* getMessageHandlerPtr(void) const;

   const MostMessage* getMostMessage() const ;

 private:

    union msg {
	   AppMessage appMsg;
	   MostMessage mostMsg;
   } theMessage;
};


//============================================================
// INLINE IMPLEMENTATION
//============================================================
inline CMessage::CMessage()
{
   memset(this, 0, sizeof(CMessage));
}

inline CMessage::CMessage(MostMessage msg)
{
	theMessage.mostMsg=msg;
}

inline CMessage::CMessage(MessageType theMsgType)
{
	memset(this, 0, sizeof(CMessage));
	theMessage.appMsg.mType = theMsgType;
}

inline void CMessage::setSenderType(unsigned char senderType)
{
	theMessage.appMsg.mType = senderType;
}

inline void CMessage::setMessageType(MessageType type)
{
	theMessage.appMsg.mType = type;
}

inline void CMessage::setSenderID(Int32 senderID)
{
   theMessage.appMsg.senderID = senderID;
}


inline void CMessage::setReceiverID(Int32 receiverID)
{
   theMessage.appMsg.receiverID = receiverID;
}


inline void CMessage::setOpcode(Int32 opcode)
{
   theMessage.appMsg.opcode = opcode;
}

inline void CMessage::setParam1(Int32 param)
{
	theMessage.appMsg.parameter1 = param;
}


inline void CMessage::setParam2(Int32 param)
{
   theMessage.appMsg.parameter2 = param;
}


inline void CMessage::setParam3(Int32 param)
{
   theMessage.appMsg.parameter3 = param;
}

inline void CMessage::setParam4(Int8* byteArrayPtr, Int8 len)
{
	Int32 i= 0;
	for( ; i<len && i<39; i++){
		theMessage.appMsg.parameter4[i]=byteArrayPtr[i];
	}
	for(i=len; i<39; i++){
		theMessage.appMsg.parameter4[i]=0;
	}
}

//inline void CMessage::setParam4(Int8* byteArrayPtr)
//{
//	Int32 i= 0;
//	for( ; i<len && i<39; i++){
//		theMessage.appMsg.parameter4[i]=byteArrayPtr[i];
//	}
//	for(i=len; i<39; i++){
//		theMessage.appMsg.parameter4[i]=0;
//	}
//}

inline void CMessage::setMessageHandlerPtr(IMessageHandler* handlerPtr)
{
	theMessage.appMsg.mMessageHandlerPtr = handlerPtr;
}

inline unsigned char CMessage::getSenderType(void) const
{
   return theMessage.appMsg.mType;
}

inline CMessage::MessageType CMessage::getMessageType(void) const
{
	return (CMessage::MessageType)theMessage.appMsg.mType;
}


inline Int32 CMessage::getSenderID(void) const
{
	return theMessage.appMsg.senderID;
}


inline Int32 CMessage::getReceiverID(void) const
{
   return theMessage.appMsg.receiverID;
}

inline Int32 CMessage::getOpcode(void) const
{
   return theMessage.appMsg.opcode;
}

inline Int32 CMessage::getParam1(void) const
{
   return theMessage.appMsg.parameter1;
}


inline Int32 CMessage::getParam2(void) const
{
   return theMessage.appMsg.parameter2;
}

inline Int32 CMessage::getParam3(void) const
{
   return theMessage.appMsg.parameter3;
}

inline const Int8* CMessage::getParam4(void) const
{
	return theMessage.appMsg.parameter4;
}

inline IMessageHandler* CMessage::getMessageHandlerPtr(void) const
{
	return theMessage.appMsg.mMessageHandlerPtr;
}

inline const MostMessage* CMessage::getMostMessage() const
{
	return &theMessage.mostMsg;
}

inline void CMessage::setMostMessage(MostMessage most) {
	theMessage.mostMsg = most;
}

inline MostMessage CMessage::getStructMostMessage(void) const{
	return theMessage.mostMsg;
}


#endif // _CMESSAGE_H
