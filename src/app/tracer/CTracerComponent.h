

#ifndef _CTRACERCOMPONENT_H
#define _CTRACERCOMPONENT_H

#ifndef _GLOBAL_H
   #include "Global.h"
#endif

#ifndef _ACOMPONENTBASE_H
   #include "AComponentBase.h"
#endif
#ifndef _CMESSAGE_H
	#include "CMessage.h"
#endif
#ifndef H_EVENTS
	#include "Events.h"
#endif
#ifndef _CCONTEXT_H
	#include "CContext.h"
#endif

	#include "CInetAddr.h"
	#include "CSockAcceptor.h"
	#include "CSockConnector.h"
	#include "CSockStream.h"
	#include "IIOStream.h"

class CTracerComponent: public AComponentBase
{
 public:
   CTracerComponent(CComponentContext& theContext);

   CSockConnector conn;
   CSockStream stream_recv, stream_send;
   CInetAddr addr_recv, addr_send;

   virtual ~CTracerComponent();

   virtual void init(void) ;
   virtual void run(void);
   void connect_and_read();

   virtual void handleMessage(const CMessage& msg);

};
#endif // _CTRACERCOMPONENT_H
