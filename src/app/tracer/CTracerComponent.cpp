

#include "CTracerComponent.h"

int counter = 0;
int MAX_COUNTER = 5;

CTracerComponent::CTracerComponent(CComponentContext& theContext) :
	AComponentBase(theContext)
{

}

CTracerComponent::~CTracerComponent()
{
}

void CTracerComponent::init(void)
{
	addr_send.set(1111, "192.168.56.1");
	addr_recv.set(1234, "192.168.56.1");
	DEBUG_PRINT("entered");
}

void CTracerComponent::run(void)
{


	DEBUG_PRINT("%s starting", mContext.getContextNamePtr());
	while (getRun())
	{
		DEBUG_PRINT("%s dispatch loop", mContext.getContextNamePtr());
		mDispatcher.dispatch(true);
	}

}

void CTracerComponent::handleMessage(const CMessage& msg)
{
	counter++;

	if(!conn.connect(stream_send, addr_send, true)) {
			DEBUG_PRINT("Send Connection error");
	} else {
			DEBUG_PRINT("Send Connection is ready -----------------");
	}
	DEBUG_PRINT("%s handle message", mContext.getContextNamePtr());

	CMessage * temp = new CMessage();

	temp->setReceiverID(msg.getReceiverID());
	temp->setSenderID(msg.getSenderID());
	temp->setOpcode(msg.getOpcode());

	const long msgsize = sizeof(CMessage);


	DEBUG_PRINT("Write to stream" );
	stream_send.write(const_cast<CMessage *>(temp), msgsize);
	stream_send.flush();
	stream_send.close();

	if(counter >= MAX_COUNTER) {
		connect_and_read();
	}

//	CMessage msg2(CMessage::Key_Event_Type);
//					msg2.setSenderID(CD_INDEX);
//					msg2.setReceiverID(HMI_INDEX);
//					msg2.setOpcode(msg.getOpcode());
//					Int8* text = (Int8*) "CD Bereit";
//					msg2.setParam4(text, strlen((char*) text));
//					CContext::getMDispContext().getNormalQueue().add(msg2, false);
}

void CTracerComponent::connect_and_read() {
		CMessage tempMsg;

		if(!conn.connect(stream_recv, addr_recv, true)) {
				DEBUG_PRINT("Recv Connection error -----------------");
		} else {
				DEBUG_PRINT("Recv Connection is ready -----------------");
		}

		while(stream_recv.read(&tempMsg, sizeof(CMessage)) > 0) {
//			printf("Component: waiting for server...\n");
//			newsockfd = recv_Acc.accept(recv_cStream);

			printf("Component: Server");
			switch(tempMsg.getReceiverID()) {
			case HMI_INDEX :
				printf("CLIENT 1 :Message Send ID: %d \n", tempMsg.getSenderID());
				printf("CLIENT 1 :Message Recv ID: %d \n", tempMsg.getReceiverID());
				printf("---------------------------------------\n");
				break;
			case CD_INDEX :
				printf("CLIENT 2 :Message Send ID: %d \n", tempMsg.getSenderID());
				printf("CLIENT 2 :Message Recv ID: %d \n", tempMsg.getReceiverID());
				printf("---------------------------------------\n");
				break;
			case TUNER_INDEX :
				printf("CLIENT 3 :Message Send ID: %d \n", tempMsg.getSenderID());
				printf("CLIENT 3 :Message Recv ID: %d \n", tempMsg.getReceiverID());
				printf("---------------------------------------\n");
				break;
			case OpenGL_INDEX :
				printf("CLIENT 4 :Message Send ID: %d \n", tempMsg.getSenderID());
				printf("CLIENT 4 :Message Recv ID: %d \n", tempMsg.getReceiverID());
				printf("---------------------------------------\n");
				break;
			default :
				printf("UNBEKANNT");
				printf("---------------------------------------\n");
			}


		}
		stream_recv.close();
}
