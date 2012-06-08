

#include "CTracerComponent.h"

CTracerComponent::CTracerComponent(CComponentContext& theContext) :
	AComponentBase(theContext)
{

}

CTracerComponent::~CTracerComponent()
{
}

void CTracerComponent::init(void)
{
	addr_send.set(1111, "localhost");

	DEBUG_PRINT("entered");
}

void CTracerComponent::run(void)
{
	conn.connect(stream_send, addr_send, true);

	DEBUG_PRINT("%s starting", mContext.getContextNamePtr());
	while (getRun())
	{
		DEBUG_PRINT("%s dispatch loop", mContext.getContextNamePtr());
		mDispatcher.dispatch(true);
	}
}

void CTracerComponent::handleMessage(const CMessage& msg)
{

	DEBUG_PRINT("%s handle message", mContext.getContextNamePtr());

	CMessage temp;

	temp.setReceiverID(msg.getReceiverID());
	temp.setSenderID(msg.getSenderID());
	temp.setOpcode(msg.getOpcode());

	const long msgsize = sizeof(CMessage);

	//GNAH! CONST = KOTZ!!! KOTZT! ABGEBEN; HEIMGEHEN!

	//stream_send.write(const_cast<&>(msg), msgsize);




//	CMessage msg2(CMessage::Key_Event_Type);
//					msg2.setSenderID(CD_INDEX);
//					msg2.setReceiverID(HMI_INDEX);
//					msg2.setOpcode(msg.getOpcode());
//					Int8* text = (Int8*) "CD Bereit";
//					msg2.setParam4(text, strlen((char*) text));
//					CContext::getMDispContext().getNormalQueue().add(msg2, false);
}
