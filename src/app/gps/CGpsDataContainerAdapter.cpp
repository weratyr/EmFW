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
	return gpsData.timestamp_usec;
}

void CGpsDataContainerAdapter::setLatitude(double const lat){
	GpsData gpsData;
	mActualGpsDC.getGpsData(gpsData);
	gpsData.latitude=lat * 1000000;
	mActualGpsDC.setGpsData(gpsData);
}

void CGpsDataContainerAdapter::setLongitude(double const lon){
	GpsData gpsData;
	mActualGpsDC.getGpsData(gpsData);
	gpsData.longitude=lon * 1000000;
	mActualGpsDC.setGpsData(gpsData);
}

void CGpsDataContainerAdapter::getActualGpsData(GpsData &gpsdata){
	//DEBUG_PRINT("TEST!!!!!!");
	mActualGpsDC.getGpsData(gpsdata);
}
