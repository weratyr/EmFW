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

#ifndef CGPSDATACONTAINER_H_
#define CGPSDATACONTAINER_H_

#ifndef CCONTAINER_H_
#include "CContainer.h"
#endif
#ifndef CMUTEX_H
#include "CMutex.h"
#endif

#ifndef CBINARYSEMAPHORE_H
#include "CBinarySemaphore.h"
#endif

#include <sys/time.h>
#include <string.h>

struct GpsData
{
	UInt32	timestamp_sec;
	UInt32	timestamp_usec;
	Int32	latitude;			// in WGS84 1/1,000,000's of a degree
	Int32	longitude;			// in WGS84 1/1,000,000's of a degree
	Int32	height;				// in 0.1 meter units
	UInt32	speed;				// in 0.01 kmh units
	UInt16	heading;			// in 0.01 degrees
	UInt16	year;
	UInt16	hdop;				// in 0.01 units
	UInt16	pdop;				// in 0.01 units
	UInt16	vdop;				// in 0.01 units
	UInt16	sat_used;
	UInt16	sat_visi;
	UInt16	hori_pos_err;
	UInt16	vert_pos_err;
	UInt16	north_speed;
	UInt16	east_speed;
	UInt16	vert_speed;
	UInt8	month;
	UInt8	day;
	UInt8	utc_hour;
	UInt8	utc_minute;
	UInt8	utc_second;
	UInt8	fix;
	UInt8	mode;
	UInt8	state;
	UInt8	flags;
	UInt8	antennaState;
	UInt8	signalQuality;
};

static const UInt32 GPS_DATA_BUFFER_SIZE=2048;

class CGpsDataContainer : public CContainer {
public:
	CGpsDataContainer();
	virtual ~CGpsDataContainer();
	void init();
	void setGpsData(const GpsData&);
	void getGpsData(GpsData&);
	void releaseNextGpsData();
private:
	GpsData mGpsData;
	CBinarySemaphore mGpsReadSemaphore;
	CBinarySemaphore mGpsWriteSemaphore;
	CMutex mGpsMutex;
};

#endif /* CGPSDATACONTAINER_H_ */
