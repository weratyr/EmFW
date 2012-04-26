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

#include "CGpsSystemNMEA.h"


CGpsSystemNMEA::CGpsSystemNMEA(const char* aGpsDevice) :
	CGpsSystem(aGpsDevice) {

}

CGpsSystemNMEA::~CGpsSystemNMEA() {

}

void CGpsSystemNMEA::init() {
	CGpsSystem::init();
	mFD = open(mDevice, O_RDONLY | O_NOCTTY | O_NDELAY);

	if (mFD == -1) {
		DEBUG_PRINT("open failed. errno: %d", errno);
		return;
	}
	memset((void *)mReadbuffer, '\0', sizeof(mReadbuffer));
	memset((void *)mSavebuffer, '\0', sizeof(mSavebuffer));
	mIsInitialized = true;
}

void CGpsSystemNMEA::cleanup() {
	CGpsSystem::cleanup();

	close(mFD);
}

// returns 	-1 on error
// 		0 if no data on mFD ready or on timeout or if mFD is not set
//		>0 mFD has data ready
Int32 CGpsSystemNMEA::waitOnPort()
{
	//DEBUG_PRINT("enter");
	Int32 maxFd,retVal;
	fd_set readfs;
	struct timeval select_timeout;

	maxFd = mFD + 1;
	FD_ZERO(&readfs);
	FD_SET(mFD, &readfs);
	select_timeout.tv_sec = GPS_READ_TIMEOUT_SEC;
	select_timeout.tv_usec = GPS_READ_TIMEOUT_USEC;
	// warten bis Daten anliegen
	Int32 res = select(maxFd, &readfs, NULL, NULL, &select_timeout);
	if (-1 == res) {
		DEBUG_PRINT("select() returned with error %d",errno);
		retVal = -1;
	} else if (0 ==  res) {
		//DEBUG_PRINT("No FD has data. Select timed out. Error: %d",errno);
		retVal= -1;
	} else {
		retVal = FD_ISSET(mFD,&readfs)?1:0;
	}
	//DEBUG_PRINT("exit %d",retVal);
	return retVal;
}

Int32 CGpsSystemNMEA::readGpsData(GpsData& gpsdata) {
	//DEBUG_PRINT("enter");
	/*
	Int32 BUFFSIZE = 255;
	char buffer[BUFFSIZE];
	char *bufptr = buffer; // current position in buf
	*/
	char *bufptr = mReadbuffer; // current position in buf
	memset((void *)mReadbuffer, '\0', sizeof(mReadbuffer));
	Int32 saveBufferLen = strlen(mSavebuffer);
	//DEBUG_PRINT("strlen(mSavebuffer): %d",saveBufferLen);
	if( saveBufferLen > 0) {
		strncpy(mReadbuffer,mSavebuffer,saveBufferLen);
		memset((void *)mSavebuffer, '\0', sizeof(mSavebuffer));
		bufptr += saveBufferLen;
		//DEBUG_PRINT("Readbuffer after restore: [%s]",mReadbuffer);
	}
	Int32 retval = 0; // Value to return
	Int32 nbytes; // Number of bytes read
	do {
		Int32 selRes = waitOnPort();

		if (selRes < 1) {
			//DEBUG_PRINT("exit %d error: %d",selRes,errno);
			return -1;
		}

		//DEBUG_PRINT("mFD is set in readfds");
		nbytes = read(mFD, bufptr, mReadbuffer+sizeof(mReadbuffer)-bufptr-1);
		//DEBUG_PRINT("%d [%s]",nbytes,bufptr);

		if(-1 == nbytes && EAGAIN != errno) {
			DEBUG_PRINT("Error in readserialdata");
			return -1;
		}
		if(-1 != nbytes) {
			//DEBUG_PRINT("Read bytes '%s'\n", bufptr);
			retval += nbytes; // Increment bytecount
			bufptr += nbytes; // Move pointer forward
		}
	} while (strchr(mReadbuffer,GPS_LINE_DELIM) == NULL );
	//DEBUG_PRINT("exit %d",retval);
	//DEBUG_PRINT("Readbuffer after read: [%s]",mReadbuffer);
	//char * line = strtok(mReadbuffer,GPS_LINE_DELIM);
	char *pLineStart = mReadbuffer;
	char *pLineEnd = mReadbuffer ;
	char line[MAX_NMEA_LEN];
	UInt32 lineLen = 0;

	// find new line character GPS_LINE_DELIM
	pLineEnd = strchr(pLineStart,GPS_LINE_DELIM);
	// while found GPS_LINE_DELIM
	while (pLineEnd != NULL) {
		lineLen = pLineEnd-pLineStart;
		//DEBUG_PRINT("Line len: %d",lineLen);
		if(lineLen <= MAX_NMEA_LEN) {
			memset(line,'\0',MAX_NMEA_LEN);
			strncpy(line,pLineStart,lineLen);
			//DEBUG_PRINT("copied act gps line: [%s]",line);
		}
		//DEBUG_PRINT("act gps line: [%s]",line);
		if(parseGPS(line,gpsdata)) {
			retval = 0;
		} else {
			retval = -1;
		}
		// end of current line AFTER DELIM (because of this +1) is start of next
		pLineStart = pLineEnd+1;
		//line = strstr(NULL,GPS_LINE_DELIM);
		pLineEnd = strchr(pLineStart,GPS_LINE_DELIM);
	}
	// save rest of readbuffer from last GPS_LINE_DELIM to end of buffer
	if(pLineStart != NULL) {
		//DEBUG_PRINT("Saving rest: [%s]",pLineStart);
		Int32 readBufferRestLen = strlen(pLineStart);
		if(readBufferRestLen > 0) {
			strncpy(mSavebuffer,pLineStart,readBufferRestLen);
			//DEBUG_PRINT("Savebuffer: [%s]",mSavebuffer);
		}
	}
	return retval;
}

bool CGpsSystemNMEA::parseGPS(const char* msg, GpsData& result) {
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
	} else {
		type = UNKNOWN;
	}
	switch(type) {
		case RMC :
			retVal = parseRMC(msg,result);
			break;
		case GGA :
			retVal = parseGGA(msg,result);
			break;
		case GSA :
			retVal = parseGSA(msg,result);
			break;
		case GSV :
			retVal = parseGSV(msg,result);
			break;
		case UNKNOWN:
			retVal=true;
			break;
		default:
			retVal = false;
			break;
	}
	return retVal;
}

const char* CGpsSystemNMEA::tokenize(const char* toSearch, char* token, char delim) {
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

int CGpsSystemNMEA::convertNMEAposToDec(const char* str, EPosType t) {
	// Breitengrad: 4735.5634 => 47° 35.5634' Nord
	char buf[8] = { 0 };
	strncpy(buf, str, t);
	buf[t] = '\0';
	int result = atoi(buf) * 1000000;
	strncpy(buf, str + t, 7);
	buf[7] = '\0';
	result += int(atof(buf) * .016666666666 * 1000000);
	return result;
}
bool CGpsSystemNMEA::parseRMC(const char* msg, GpsData& result) {
	char buf[256];

	memset(buf, 0, sizeof(buf));
	memset(&result, 0, sizeof(result));

	/* ############# $GPRMC Message ############
	 *
	 */
	//DEBUG_PRINT("parse line: [%s]",msg);
	char* pStart = const_cast<char *> (strstr(msg, "$GPRMC")); // bad !!

	if (pStart == NULL) {
		DEBUG_PRINT("Not a RMC sentence");
		return false;
	}

	if (pStart != NULL) {
		char* pEnd = pStart+strlen(msg);
		if (pEnd != NULL && pEnd - pStart < (int) sizeof(buf)) {
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
					// Not used
					break;
				case 1: // time
					result.utc_hour = (int) (atoi(token) / 10000);
					result.utc_minute = (int) (atoi(token) / 100)	- (result.utc_hour * 100);
					result.utc_second = atoi(token) - (result.utc_hour * 10000)	- (result.utc_minute * 100);
					break;
				case 2: // Valid Flag
					// Not used
					break;
				case 3: // latitude
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
				case 7: // velocity
					// Not used
					// IN $GPVTG it is in km/h
					break;
				case 8: // direction
					// Not used
					break;
				case 9: // date
					result.day = (int) (atoi(token) / 10000);
					result.month = (int) (atoi(token) / 100) - (result.day * 100);
					result.year = (atoi(token) - (result.day * 10000) - (result.month * 100)) + 2000;
					break;
				case 10: // Misswise
					// Not used
					break;
				case 11: // Misswise direction
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
bool CGpsSystemNMEA::parseGGA(const char* msg, GpsData& result) {
	//DEBUG_PRINT("not implemented yet");
	return true;
}
bool CGpsSystemNMEA::parseGSA(const char* msg, GpsData& result) {
	//DEBUG_PRINT("not implemented yet");
	return true;
}
bool CGpsSystemNMEA::parseGSV(const char* msg, GpsData& result) {
	//DEBUG_PRINT("not implemented yet");
	return true;
}
