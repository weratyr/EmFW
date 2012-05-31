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

#ifndef _CGPSCOMPONENT_H
#define _CGPSCOMPONENT_H

#ifndef _GLOBAL_H
   #include "Global.h"
#endif

#ifndef _CCONTEXT_H
   #include "CContext.h"
#endif

#ifndef _ACOMPONENTBASE_H
   #include "AComponentBase.h"
#endif

#ifndef _CDISPATCHER_H
   #include "CDispatcher.h"
#endif

#ifndef _CMESSAGE_H
   #include "CMessage.h"
#endif

#ifndef CGPSDATACONTAINERADAPTER_H_
	#include "CGpsDataContainerAdapter.h"
#endif

#ifndef CGPSDATACONTAINERSCCESSOR_H_
	#include "CGpsDataContainerAccessor.h"
#endif

#ifndef _EVENTS_H
	#include "Events.h"
#endif

#ifndef CGPSNMEAPARSER_H
	#include "CGpsNMEAParser.h"
#endif

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <cstdlib>

class CGpsComponent: public AComponentBase
{
public:
	CGpsComponent(CComponentContext& theContext);
	virtual ~CGpsComponent();

	virtual void init(void);
	virtual void run(void);

	virtual void handleMessage(const CMessage&);

private:
  void sendPositionMessage();

  CGpsDataContainerAdapter mGpsDCAccessor;
  CGpsNMEAParser mGpsNMEAParser;
};

#endif /* CGPSCOMPONENT_H_ */
