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

/**
 * Class Declaration for CMiniComDriver
 *
 * Represents the framework component to
 * provide the integration of the mini commander
 * device by use of the CMiniCommander class.
 *
 */
#ifndef _CMINICOMDRIVER_H
#define _CMINICOMDRIVER_H


#ifndef _IRUNNABLE_H
#include "IRunnable.h"
#endif
#ifndef _CMESSAGE_H
#include "CMessage.h"
#endif
#ifndef _CMINICOMMANDER_H
#include "CMiniCommander.h"
#endif

class CMiniComDriver : public IRunnable
{
public:
	CMiniComDriver();
	virtual ~CMiniComDriver();

	virtual void init(void){}
	virtual void run(void);
	virtual void stop(void);
	virtual void cleanup(void){}
	virtual void reRun();
	virtual void prepareForShutdown(){}
	virtual void goToStandby(){}

	virtual void handleMessage(const CMessage& msg);

private:
   CMiniCommander* mMiniCom;
};

#endif
