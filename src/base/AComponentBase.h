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

#ifndef _ACOMPONENTBASE_H
#define _ACOMPONENTBASE_H

#ifndef _IRUNNABLE_H
	#include "IRunnable.h"
#endif

#ifndef _IMESSAGEHANDLER_H
	#include "IMessageHandler.h"
#endif

#ifndef _CCOMPONENTCONTEXT_H
	#include "CComponentContext.h"
#endif

#ifndef _CDISPATCHER_H
	#include "CDispatcher.h"
#endif

class AComponentBase : public IRunnable, public IMessageHandler
{
public:
	AComponentBase(CComponentContext& theContext);
	virtual ~AComponentBase();

	/*
	 * from IRunnable
	 */
	//have to implemented in derived classes
	virtual void init(void) = 0;
	virtual void run(void)	= 0;

	//could be overwritten in derived classes
	virtual void cleanup(void)	{}
	virtual void prepareForShutdown(void) {}
	virtual void stop();
	virtual void reRun()	{}
	virtual void goToStandby()	{}

	/*
	 * from IMessageHandler
	 */
	//have to implemented in derived classes
	virtual void handleMessage(const CMessage& msg)	= 0;

protected:
	inline bool getRun() { return mRun; }

	CComponentContext& mContext;
	CDispatcher mDispatcher;
	bool mRun;
};

#endif /* _ACOMPONENTBASE_H */
