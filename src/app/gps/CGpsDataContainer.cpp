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

#include "CGpsDataContainer.h"

CGpsDataContainer::CGpsDataContainer() {
}

CGpsDataContainer::~CGpsDataContainer() {
}

bool CGpsDataContainer::setGpsData(const GpsData& aGpsData)
{
	mMutex.take(true);
	memcpy(&mGpsData,&aGpsData,sizeof(GpsData));
/*
	mGpsData.timestamp = aGpsData.timestamp;
	mGpsData.state = aGpsData.state;
	mGpsData.flags = aGpsData.flags;
	mGpsData.antennaState = aGpsData.antennaState;
	mGpsData.signalQuality = aGpsData.signalQuality;
	mGpsData.latitude = aGpsData.latitude;
	mGpsData.longitude = aGpsData.longitude;
	mGpsData.height = aGpsData.height;
	mGpsData.speed = aGpsData.speed;
	mGpsData.heading = aGpsData.heading;
	mGpsData.year = aGpsData.year;
	mGpsData.month = aGpsData.month;
	mGpsData.day = aGpsData.day;
	mGpsData.utc_hour = aGpsData.utc_hour;
	mGpsData.utc_minute = aGpsData.utc_minute;
	mGpsData.utc_second = aGpsData.utc_second;
	mGpsData.fix = aGpsData.fix;
	mGpsData.hdop = aGpsData.hdop;
	mGpsData.pdop = aGpsData.pdop;
	mGpsData.vdop = aGpsData.vdop;
	mGpsData.sat_used = aGpsData.sat_used;
	mGpsData.sat_visi = aGpsData.sat_visi;
	mGpsData.hori_pos_err = aGpsData.hori_pos_err;
	mGpsData.vert_pos_err = aGpsData.vert_pos_err;
	mGpsData.north_speed = aGpsData.north_speed;
	mGpsData.east_speed = aGpsData.east_speed;
	mGpsData.vert_speed = aGpsData.vert_speed;
*/
	mMutex.give();
	return true;
}
bool CGpsDataContainer::getGpsData(GpsData& aGpsData)
{
	mMutex.take(true);
	memcpy(&aGpsData,&mGpsData,sizeof(GpsData));
/*
	aGpsData.timestamp = mGpsData.timestamp;
	aGpsData.state = mGpsData.state;
	aGpsData.flags = mGpsData.flags;
	aGpsData.antennaState = mGpsData.antennaState;
	aGpsData.signalQuality = mGpsData.signalQuality;
	aGpsData.latitude = mGpsData.latitude;
	aGpsData.longitude = mGpsData.longitude;
	aGpsData.height = mGpsData.height;
	aGpsData.speed = mGpsData.speed;
	aGpsData.heading = mGpsData.heading;
	aGpsData.year = mGpsData.year;
	aGpsData.month = mGpsData.month;
	aGpsData.day = mGpsData.day;
	aGpsData.utc_hour = mGpsData.utc_hour;
	aGpsData.utc_minute = mGpsData.utc_minute;
	aGpsData.utc_second = mGpsData.utc_second;
	aGpsData.fix = mGpsData.fix;
	aGpsData.hdop = mGpsData.hdop;
	aGpsData.pdop = mGpsData.pdop;
	aGpsData.vdop = mGpsData.vdop;
	aGpsData.sat_used = mGpsData.sat_used;
	aGpsData.sat_visi = mGpsData.sat_visi;
	aGpsData.hori_pos_err = mGpsData.hori_pos_err;
	aGpsData.vert_pos_err = mGpsData.vert_pos_err;
	aGpsData.north_speed = mGpsData.north_speed;
	aGpsData.east_speed = mGpsData.east_speed;
	aGpsData.vert_speed = mGpsData.vert_speed;
*/
	mMutex.give();
	return true;
}
