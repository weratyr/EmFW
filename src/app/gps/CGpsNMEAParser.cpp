
#include "CGpsNMEAParser.h"


CGpsNMEAParser::CGpsNMEAParser() {}

CGpsNMEAParser::~CGpsNMEAParser() {}


Int32 CGpsNMEAParser::parseGPS(const char* msg, GpsData& result) {
	timeval time;
	time.tv_sec=0;
	time.tv_usec=0;
	if(gettimeofday(&time, NULL) == 0){
		result.timestamp_sec = time.tv_sec;
		result.timestamp_usec = time.tv_usec;
	}
	ESentenceType type;
	bool retVal = false;
	if (strstr(msg,"$GPRMC") != NULL ) {
		type = RMC;
	} else if (strstr(msg,"$GPGGA") != NULL ) {
		type = GGA;
	} else if (strstr(msg,"$GPGSA") != NULL ) {
		type = GSA;
	} else if (strstr(msg,"$GPGSV") != NULL ) {
		type = GSV;
	} else if (strstr(msg,"$GPVTG") != NULL ) {
		type = VTG;
	} else {
		type = UNKNOWN;
	}
	switch(type) {
		case RMC :
//			DEBUG_PRINT("parseRMC");
			retVal = parseRMC(msg,result);
			break;
		case GGA :
//			DEBUG_PRINT("parseGGA");
			//retVal = parseGGA(msg,result);
			break;
		case GSA :
//			DEBUG_PRINT("parseGSA");
			//retVal = parseGSA(msg,result);
			break;
		case GSV :
//			DEBUG_PRINT("parseGSV");
			//retVal = parseGSV(msg,result);
			break;
		case VTG :
//			DEBUG_PRINT("parseVTG");
			//retVal = parseVTG(msg, result);
			break;
		case UNKNOWN:
//			DEBUG_PRINT("UNKNOWN\n");
			retVal=false;
			break;
		default:
			retVal = false;
			break;
	}
	return retVal;
}

const char* CGpsNMEAParser::tokenize(const char* toSearch, char* token, char delim) {
	char* hit = const_cast<char *>(strchr(toSearch, delim));
	if (hit != NULL) {
		size_t len = hit - toSearch;
		if (len < TOKENMAXSIZE) {
			strncpy(token, toSearch, len);
			token[len] = '\0';
		}
		++hit; // add up 1 char for delim
	}
	return hit;
}

Int32 CGpsNMEAParser::convertNMEAposToDec(const char* str, EPosType t) {
	// Breitengrad: 4735.5634 => 47° 35.5634' Nord
	char buf[8] = { 0 };
	strncpy(buf, str, t);
	buf[t] = '\0';
	Int32 result = atoi(buf) * 1000000;
	strncpy(buf, str + t, 7);
	buf[7] = '\0';
	result += Int32(atof(buf) * .016666666666 * 1000000);
	return result;
}
Int32 CGpsNMEAParser::parseRMC(const char* msg, GpsData& result) {
	char buf[256];
	memset(buf, 0, sizeof(buf));
	char* pStart = const_cast<char *> (strstr(msg, "$GPRMC")); // bad !!

	if (pStart == NULL) {
		DEBUG_PRINT("Not a RMC sentence");
		return false;
	}

	if (pStart != NULL) {
		char* pEnd = pStart+strlen(msg);
		if (pEnd != NULL && pEnd - pStart < (Int32) sizeof(buf)) {
			strncpy(buf, pStart, pEnd - pStart);
			buf[pEnd - pStart] = '\0';

			// now tokenize for determining the contained values
			char token[TOKENMAXSIZE];
			const char* tosearch = buf; // search pointer
			const char* nextStart = NULL;

			size_t counter = 0;
			while ((nextStart = tokenize(tosearch, token, ',')) != NULL) {
				switch (counter) {
				case 0: //$GPRMC
					//not used
					break;
				case 1: // time
					result.utc_hour = (Int32) (atoi(token) / 10000);
					result.utc_minute = (Int32) (atoi(token) / 100)	- (result.utc_hour * 100);
					result.utc_second = atoi(token) - (result.utc_hour * 10000)	- (result.utc_minute * 100);
					break;
				case 2: // Valid Flag
					result.flags = *token;
					break;
				case 3: // latitude
					//DEBUG_PRINT("POS_LAT %s\n",token);
					result.latitude = convertNMEAposToDec(token, POS_LAT);
					break;
				case 4: // latitude direction
					// Not used
					break;
				case 5: // longitude
					result.longitude = convertNMEAposToDec(token, POS_LON);
					break;
				case 6: // longitude direction
					// Not used
					break;
				case 7: // velocity in knots (1kn = 1,852 km/h)
					result.speed = (1.852 * (Int32) atoi(token));
					// in $GPVTG it is in km/h
					break;
				case 8: // direction in degrees
					result.heading = (Int32) (atof(token) * 100);
					break;
				case 9: // date
					result.day = (Int32) (atoi(token) / 10000);
					result.month = (Int32) (atoi(token) / 100) - (result.day * 100);
					result.year = (atoi(token) - (result.day * 10000) - (result.month * 100)) + 2000;
					break;
				case 10: // Misswise
					// Not used
					break;
				case 11:
					break;
				case 12: //Mode*Checksum - new in NMEA 2.3:
					/*
					Positioning Mode:
					A=autonomous (selbst)
					D=differential
					E=estimated (dead reckoning)
					N=not valid (ungültig)
					S=simulator
					*/
					result.mode = (Int32) atoi(token);
					break;
				default:
					break;
				} // switch
				tosearch = nextStart;
				++counter;
			} // while
		} // if
	}

	return true;
}

Int32 CGpsNMEAParser::parseGGA(const char* msg, GpsData& result) {
	char buf[256];
	memset(buf, 0, sizeof(buf));
	/* #############
	 * $GPGAA Message ############
	 *
	 */
	char* pStart = const_cast<char *> (strstr(msg, "$GPGGA")); // bad !!

	if (pStart == NULL) {
		DEBUG_PRINT("Not a GGA sentence");
		return false;
	}

	if (pStart != NULL) {
		char* pEnd = pStart+strlen(msg);
		if (pEnd != NULL && pEnd - pStart < (Int32) sizeof(buf)) {
			strncpy(buf, pStart, pEnd - pStart);
			buf[pEnd - pStart] = '\0';

			// now tokenize for determining the contained values
			char token[TOKENMAXSIZE];
			const char* tosearch = buf; // search pointer
			const char* nextStart = NULL;

			size_t counter = 0;
			while ((nextStart = tokenize(tosearch, token, ',')) != NULL) {
				switch (counter) {
				case 0: //$GPGAA
					// Not used
					break;
				case 1: // time
					result.utc_hour = (Int32) (atoi(token) / 10000);
					result.utc_minute = (Int32) (atoi(token) / 100)	- (result.utc_hour * 100);
					result.utc_second = atoi(token) - (result.utc_hour * 10000)	- (result.utc_minute * 100);
					break;
				case 2: // latitude
					result.latitude = convertNMEAposToDec(token, POS_LAT);
					break;
				case 3: // latitude direction
					// Not used
					break;
				case 4: // longitude
					result.longitude = convertNMEAposToDec(token, POS_LON);
					break;
				case 5: // longitude direction
					// Not used
					break;
				case 6: // GPS Qualitiy Indicator
					//0=invalid, 1=GPS fix (SPS), 2=DGPS fix, 3=PPS,
					//4=Real Time Kinematic, 5=Float RTK, 6=estimated(dead reckoning)
					//7=Manual input mode, 8=Simulation mode
					result.signalQuality = (Int32) atoi(token);
					break;
				case 7: // Number of visible satellites 00-12
					result.sat_visi = (Int32) atoi(token);
					break;
				case 8: // Horizontal Dilution of precision (meters)
					result.hdop = (Int32) atoi(token);
					break;
				case 9: // Altitude above/below mean-sea-level (geoid) (in meters)

					break;
				case 10: // height of geoid above WGS84 (height geoid - height ellipsoid in meters)
					// Not used
					break;
				default:
					break;
				} // switch
				tosearch = nextStart;
				++counter;
			} // while
		} // if
	}
	return true;
}

Int32 CGpsNMEAParser::parseGSA(const char* msg, GpsData& result) {
	//DEBUG_PRINT("not implemented yet");
	return 1;
}

Int32 CGpsNMEAParser::parseGSV(const char* msg, GpsData& result) {
	//DEBUG_PRINT("not implemented yet");
	return 1;
}

Int32 CGpsNMEAParser::parseVTG(const char* msg, GpsData& result){
	char buf[256];
	memset(buf, 0, sizeof(buf));
	/* #############
	 * $GPVTG Message ############
	 *
	 */
	char* pStart = const_cast<char *> (strstr(msg, "$GPVTG")); // bad !!

	if (pStart == NULL) {
		DEBUG_PRINT("Not a VTG sentence");
		return false;
	}

	if (pStart != NULL) {
		char* pEnd = pStart+strlen(msg);
		if (pEnd != NULL && pEnd - pStart < (Int32) sizeof(buf)) {
			strncpy(buf, pStart, pEnd - pStart);
			buf[pEnd - pStart] = '\0';

			// now tokenize for determining the contained values
			char token[TOKENMAXSIZE];
			const char* tosearch = buf; // search pointer
			const char* nextStart = NULL;
/*
			$GPVTG,0.0,T,359.6,M,0.0,N,0.0,K*47
			       ^     ^       ^     ^
			       |     |       |     |
			       |     |       |     Geschwindigkeit über Grund in km/h (K)
			       |     |       |
			       |     |       Geschwindigkeit über Grund in Knoten (N)
			       |     |
			       |     Kurs (magnetisch, M)
			       |
			       Kurs (wahr, T)

			$--VTG,x.x,T,x.x,M,x.x,N,x.x,K
			x.x,T = Track, degrees True
			x.x,M = Track, degrees Magnetic
			x.x,N = Speed, knots
			x.x,K = Speed, Km/hr
*/
			size_t counter = 0;
			while ((nextStart = tokenize(tosearch, token, ',')) != NULL) {
				switch (counter) {
				case 0: //$GPVTG
					// Not used
					break;
				case 1: //True course over ground (COG) in degrees (000 to 359)
					//result.tcog = (int) atoi(token);
					break;
				case 2: //True course over ground indicator (always 'T')
					// Not used
					break;
				case 3: //Magnetic course over ground in degrees (000 to 359)
					//result.mcog = (int) atoi(token);
					break;
				case 4: //Magnetic course over ground indicator (always 'M')
					// Not used
					break;
				case 5: //Speed over ground in knots
					// result.knotspeed = (int) atoi(token);
					break;
				case 6: //Speed over ground in knots indicator (always 'N')
					// Not used
					break;
				case 7: //Speed over ground in km/h
					result.speed = (Int32) atoi(token);
					break;
				case 8: //Speed over ground in km/h indicator (always 'K')
					// Not used
					break;
				default:
//					DEBUG_TRACE("not able to handle position %d for elemnent %s", counter, token);
					break;
				} // switch
				tosearch = nextStart;
				++counter;
			} // while
		} // if
	}
	return true;
}
