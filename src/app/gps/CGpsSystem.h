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

#ifndef _CGPSSYSTEM_H
#define _CGPSSYSTEM_H

#ifndef _GLOBAL_H
   #include "Global.h"
#endif

#ifndef CGPSDATACONTAINER_H_
	#include "CGpsDataContainer.h"
#endif

static const UInt32 READ_BUFFSIZE = 1024;
static const UInt32 SAVE_BUFFSIZE = 128;
static const Int32 GPS_LINE_DELIM = '\n';
static const UInt32 MAX_NMEA_LEN = 80;

class CGpsSystem {

public:
	CGpsSystem(const char* );
	virtual ~CGpsSystem();

	// initialisieren des Subsystems
	virtual void init();

	// aufräumen
	virtual void cleanup();

	// Gps Daten auslesen
	virtual Int32 readGpsData(GpsData&) = 0;
	bool isInitialized();
	const char* mDevice;
protected:
	bool mIsInitialized;
	char mReadbuffer[READ_BUFFSIZE];
	char mSavebuffer[SAVE_BUFFSIZE];
};


#endif //_CGPSSYSTEM_H
