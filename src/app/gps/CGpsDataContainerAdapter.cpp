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

#include "CGpsDataContainerAdapter.h"

CGpsDataContainerAdapter::CGpsDataContainerAdapter(CGpsDataContainer& container)
:mActualGpsDC(container) {
}

CGpsDataContainerAdapter::~CGpsDataContainerAdapter() {
}

double CGpsDataContainerAdapter::getLatitude() const
{
	GpsData gpsData;
	mActualGpsDC.getGpsData(gpsData);
	return ((double) gpsData.latitude) / 1000000.0;
}

double CGpsDataContainerAdapter::getLongitude() const
{
	GpsData gpsData;
	mActualGpsDC.getGpsData(gpsData);
	return ((double) gpsData.longitude) / 1000000.0;
}
UInt32	CGpsDataContainerAdapter::getTimestamp() const
{
	GpsData gpsData;
	mActualGpsDC.getGpsData(gpsData);
	return gpsData.timestamp;
}
UInt8	CGpsDataContainerAdapter::getState() const
{
	GpsData gpsData;
	mActualGpsDC.getGpsData(gpsData);
	return gpsData.state;
}
UInt8	CGpsDataContainerAdapter::getFlags() const
{
	GpsData gpsData;
	mActualGpsDC.getGpsData(gpsData);
	return gpsData.flags;
}
UInt8	CGpsDataContainerAdapter::getAntennaState() const
{
	GpsData gpsData;
	mActualGpsDC.getGpsData(gpsData);
	return gpsData.antennaState;
}
UInt8	CGpsDataContainerAdapter::getSignalQuality() const
{
	GpsData gpsData;
	mActualGpsDC.getGpsData(gpsData);
	return gpsData.signalQuality;
}
Int32	CGpsDataContainerAdapter::getHeight() const
{
	GpsData gpsData;
	mActualGpsDC.getGpsData(gpsData);
	return gpsData.height;
}
UInt32	CGpsDataContainerAdapter::getSpeed() const
{
	GpsData gpsData;
	mActualGpsDC.getGpsData(gpsData);
	return gpsData.speed;
}
UInt16	CGpsDataContainerAdapter::getHeading() const
{
	GpsData gpsData;
	mActualGpsDC.getGpsData(gpsData);
	return gpsData.heading;
}
UInt16	CGpsDataContainerAdapter::getYear() const
{
	GpsData gpsData;
	mActualGpsDC.getGpsData(gpsData);
	return gpsData.year;
}
UInt8	CGpsDataContainerAdapter::getMonth() const
{
	GpsData gpsData;
	mActualGpsDC.getGpsData(gpsData);
	return gpsData.month;
}
UInt8	CGpsDataContainerAdapter::getDay() const
{
	GpsData gpsData;
	mActualGpsDC.getGpsData(gpsData);
	return gpsData.day;
}
UInt8	CGpsDataContainerAdapter::getUtcHour() const
{
	GpsData gpsData;
	mActualGpsDC.getGpsData(gpsData);
	return gpsData.utc_hour;
}
UInt8	CGpsDataContainerAdapter::getUtcMinute() const
{
	GpsData gpsData;
	mActualGpsDC.getGpsData(gpsData);
	return gpsData.utc_minute;
}
UInt8	CGpsDataContainerAdapter::getUtcSecond() const
{
	GpsData gpsData;
	mActualGpsDC.getGpsData(gpsData);
	return gpsData.utc_second;
}
UInt8	CGpsDataContainerAdapter::getFix() const
{
	GpsData gpsData;
	mActualGpsDC.getGpsData(gpsData);
	return gpsData.fix;
}
UInt16	CGpsDataContainerAdapter::getHdop() const
{
	GpsData gpsData;
	mActualGpsDC.getGpsData(gpsData);
	return gpsData.hdop;
}
UInt16	CGpsDataContainerAdapter::getPdop() const
{
	GpsData gpsData;
	mActualGpsDC.getGpsData(gpsData);
	return gpsData.pdop;
}
UInt16	CGpsDataContainerAdapter::getVdop() const
{
	GpsData gpsData;
	mActualGpsDC.getGpsData(gpsData);
	return gpsData.vdop;
}
UInt16	CGpsDataContainerAdapter::getSatUsed() const
{
	GpsData gpsData;
	mActualGpsDC.getGpsData(gpsData);
	return gpsData.sat_used;
}
UInt16	CGpsDataContainerAdapter::getSatVisi() const
{
	GpsData gpsData;
	mActualGpsDC.getGpsData(gpsData);
	return gpsData.sat_visi;
}
UInt16	CGpsDataContainerAdapter::getHoriPosErr() const
{
	GpsData gpsData;
	mActualGpsDC.getGpsData(gpsData);
	return gpsData.hori_pos_err;
}
UInt16	CGpsDataContainerAdapter::getVertPosErr() const
{
	GpsData gpsData;
	mActualGpsDC.getGpsData(gpsData);
	return gpsData.vert_pos_err;
}
UInt16	CGpsDataContainerAdapter::getNorthSpeed() const
{
	GpsData gpsData;
	mActualGpsDC.getGpsData(gpsData);
	return gpsData.north_speed;
}
UInt16	CGpsDataContainerAdapter::getEastSpeed() const
{
	GpsData gpsData;
	mActualGpsDC.getGpsData(gpsData);
	return gpsData.east_speed;
}
UInt16	CGpsDataContainerAdapter::getVertSpeed() const
{
	GpsData gpsData;
	mActualGpsDC.getGpsData(gpsData);
	return gpsData.vert_speed;
}
