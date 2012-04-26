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

#ifndef CGPSDATACONTAINERADAPTER_H_
#define CGPSDATACONTAINERADAPTER_H_

#ifndef CGPSDATACONTAINER_H_
	#include "CGpsDataContainer.h"
#endif

class CGpsDataContainerAdapter {
public:
	CGpsDataContainerAdapter(CGpsDataContainer&);
	virtual ~CGpsDataContainerAdapter();

	UInt32	getTimestamp() const;
	UInt8	getState() const;
	UInt8	getFlags() const;
	UInt8	getAntennaState() const;
	UInt8	getSignalQuality() const;
	Int32	getHeight() const;
	UInt32	getSpeed() const;
	UInt16	getHeading() const;
	UInt16	getYear() const;
	UInt8	getMonth() const;
	UInt8	getDay() const;
	UInt8	getUtcHour() const;
	UInt8	getUtcMinute() const;
	UInt8	getUtcSecond() const;
	UInt8	getFix() const;
	UInt16	getHdop() const;
	UInt16	getPdop() const;
	UInt16	getVdop() const;
	UInt16	getSatUsed() const;
	UInt16	getSatVisi() const;
	UInt16	getHoriPosErr() const;
	UInt16	getVertPosErr() const;
	UInt16	getNorthSpeed() const;
	UInt16	getEastSpeed() const;
	UInt16	getVertSpeed() const;
	double getLatitude()const;
	double getLongitude()const;

private:
	CGpsDataContainer& mActualGpsDC;
};

#endif /* CGPSDATACONTAINERADAPTER_H_ */
