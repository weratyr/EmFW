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

#ifndef _H_CMAINDISPATCHER
#define _H_CMAINDISPATCHER

#ifndef _GLOBAL_H
#include "Global.h"
#endif

#ifndef _CCONTEXT_H
#include "CContext.h"
#endif

#ifndef _ACOMPONENTBASE_H
#include "AComponentBase.h"
#endif

#ifndef H_EVENTS
#include "Events.h"
#endif

class CMainDispatcher: public AComponentBase
{

public:

	CMainDispatcher(CComponentContext& theContext);

	virtual ~CMainDispatcher();

	virtual void init(void);
	virtual void run(void);

	virtual void handleMessage(const CMessage& msg);


	//TODO prio 3 :: MOST funktionalitaet hinzufuegen (server/client, abh. von hardware) (bk)
	void handleMostMessage(const CMessage& msg);
	void handleAppMessage(const CMessage& msg);
	void handleFromMostMessage(const CMessage& msg);
	void handleKeyMessage(const CMessage& msg);

};

#endif // _H_CMAINDISPATCHER
