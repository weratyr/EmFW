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

#include "CGpsSystemQnxSh4.h"


CGpsSystemQnxSh4::CGpsSystemQnxSh4(const char* aGpsDevice) :
	CGpsSystem(aGpsDevice) {

}

CGpsSystemQnxSh4::~CGpsSystemQnxSh4() {

}

void CGpsSystemQnxSh4::init() {
	CGpsSystem::init();

	this->mFP = fopen(this->mDevice, "rb");

	if(mFP == NULL) {
		DEBUG_PRINT("open failed. errno: %d", errno);
		return;
	}
	mIsInitialized = true;
}

void CGpsSystemQnxSh4::cleanup() {
	CGpsSystem::cleanup();

	fclose(mFP);
}

Int32 CGpsSystemQnxSh4::readGpsData(GpsData& gpsdata) {
	// Device file liefert Daten im Format von GpsData automatisch
	// ein Datensatz pro fread
	fread((void *) &gpsdata, sizeof(GpsData), 1, mFP);
	//DEBUG_PRINT("GPS data -- Lat: %d Lon: %d date: %d/%d/%d time: %d:%d:%d vdop: %d, hdop: %d, fix: %d, num sats used: %d, num sats visi: %d, state: %d"
	//		,gpsdata.latitude,gpsdata.longitude,gpsdata.year,gpsdata.month,gpsdata.day,gpsdata.utc_hour,gpsdata.utc_minute,gpsdata.utc_second,gpsdata.vdop,gpsdata.hdop, gpsdata.fix, gpsdata.sat_used, gpsdata.sat_visi,gpsdata.state);
	return 0;
}
