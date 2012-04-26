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

#ifndef _CGPSSYSTEMNMEA_H
#define _CGPSSYSTEMNMEA_H

#ifndef _CGPSSYSTEM_H
	#include "CGpsSystem.h"
#endif

#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define TOKENMAXSIZE 32

static const Int32 GPS_READ_TIMEOUT_SEC = 2;
static const Int32 GPS_READ_TIMEOUT_USEC = 0;

class CGpsSystemNMEA: public CGpsSystem {

public:
	CGpsSystemNMEA(const char*);
	virtual ~CGpsSystemNMEA();

	void init();
	void cleanup();
	Int32 readGpsData(GpsData&);

private:

	enum EPosType {
		POS_LAT = 2, POS_LON = 3
	};

	enum ESentenceType {
		RMC,GGA,GSV,GSA,UNKNOWN
	};

	Int32 mFD;

	bool parseGPS(const char* msg, GpsData& result);
	bool parseRMC(const char* msg, GpsData& result);
	bool parseGGA(const char* msg, GpsData& result);
	bool parseGSV(const char* msg, GpsData& result);
	bool parseGSA(const char* msg, GpsData& result);
	const char* tokenize(const char* toSearch, char* token, char delim);
	int convertNMEAposToDec(const char* str, EPosType t);
	Int32 waitOnPort(void);
};


#endif //_CGPSSYSTEMLINUXX86_H
