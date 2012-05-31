/*
 * CGpsNMEAParser.h
 *
 *  Created on: 01.04.2011
 *      Author: istctsche
 */


#ifndef CGPSNMEAPARSER_H
#define CGPSNMEAPARSER_H

#ifndef GLOBAL_H
	#include "Global.h"
#endif

#ifndef CGPSDATACONTAINER_H
	#include "CGpsDataContainer.h"
#endif

#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define TOKENMAXSIZE 32
static const UInt32 MAX_NMEA_LEN = 80;

class CGpsNMEAParser{

public:
	CGpsNMEAParser();
	virtual ~CGpsNMEAParser();
	Int32 parseGPS(const char* msg, GpsData& result);

private:

	enum EPosType {
		POS_LAT = 2, POS_LON = 3
	};

	enum ESentenceType {
		RMC,GGA,GSV,GSA,VTG,UNKNOWN
	};

	Int32 mFD;

	Int32 parseRMC(const char* msg, GpsData& result);
	Int32 parseGGA(const char* msg, GpsData& result);
	Int32 parseGSV(const char* msg, GpsData& result);
	Int32 parseGSA(const char* msg, GpsData& result);
	Int32 parseVTG(const char* msg, GpsData& result);
	const char* tokenize(const char* toSearch, char* token, char delim);
	int convertNMEAposToDec(const char* str, EPosType t);
};




#endif //_CGPSSYSTEMLINUXX86_H

