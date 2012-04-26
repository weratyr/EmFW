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

#ifndef CGPSDATACONTAINERACCESSOR_H_
	#include "CGpsDataContainerAccessor.h"
#endif

#ifndef _EVENTS_H
	#include "Events.h"
#endif

#ifndef _CGPSSYSTEM_H
	#include "CGpsSystem.h"
#endif

#ifdef TARGET_D1
	#include "CGpsSystemQnxSh4.h"
	#define GPS_DEVICE "/dev/navi/sensor/GPS/AllGps"
#else
	#include "CGpsSystemNMEA.h"
	#ifdef __linux__
		#define GPS_DEVICE "/dev/ttyACM0" //u-blox5
		//#define GPS_DEVICE "/dev/ttyUSB0" //SiRF Star III
	#else //Qnx_x86
		#define GPS_DEVICE "/dev/ser2"
	#endif
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
  void updateGpsDC();

  static CGpsSystem* sGpsSystem;
  const char* mDevice;
  CGpsDataContainerAccessor mGpsDCAccessor;
  GpsData mGpsData;
};

#endif /* CGPSCOMPONENT_H_ */
