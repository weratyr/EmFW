#include "CGpsFileParserComponent.h"
const char CGpsFileParserComponent::sDelim = '\t';

CGpsFileParserComponent::CGpsFileParserComponent(CComponentContext& theContext, char* fileName) :
		AComponentBase(theContext), mGpsDCAccessor(static_cast<CGpsDataContainer&>(CContext::getGpsContext().getContainer())), mFileName(fileName) {
}

CGpsFileParserComponent::~CGpsFileParserComponent() {
}

void CGpsFileParserComponent::init(void) {
	DEBUG_PRINT("entered");
	mRights = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | S_IWGRP | S_IWOTH;
	DEBUG_PRINT("Open file \"%s\" for reading",mFileName);
	mFd = open(mFileName, O_RDONLY);

	if (mFd < 0) {
		fprintf(stderr,
				"WARNING: file not opened: \"%s\" - fd: %i, Error: %s\n",
				mFileName, mFd, strerror(errno));
	}
}

void CGpsFileParserComponent::run(void) {
	bool eof = false;
	while (!eof) {
		mDispatcher.dispatch(false);
		UInt32 writePos = 0;
		char readBuffer[GPS_DATA_BUFFER_SIZE];
		memset(readBuffer, 0, GPS_DATA_BUFFER_SIZE);
		while ((writePos == 0 || readBuffer[(writePos - 1)] != sDelim) && !eof) {
			Int32 gelesen = read(mFd, &readBuffer[writePos], 1);
			if (gelesen == 0) //ende der Datei erreicht
					{
				eof = true;
			}
			if (readBuffer[writePos] == 0 || gelesen == -1) {
				gelesen = 0;
			}
			if (gelesen == 1) {
				writePos++;
			}
		}
		char type[3];
		UInt32 sec, usec;
		sscanf(readBuffer, "%10d %6d %s", &sec, &usec, type);

		/*
		 * TODO:
		 * Wenn der gelesene type GPS ist:
		 * 1. parseGPSData
		 * 2. geparste GPS Daten in Datencontainer schreiben
		 * 3. Signalisieren, dass neue GPS Werte verfügbar sind
		 */

		//--- Parse GPS Daten
		GpsData gpsdata;
		timeval aTimestamp;
		parseGpsData(gpsdata, readBuffer, GPS_DATA_BUFFER_SIZE, aTimestamp);
		//--- in DC schreiben
		//DEBUG_PRINT("--- GPSDATA: Longitude: %d, Latitude: %d", gpsdata.longitude, gpsdata.latitude);
		mGpsDCAccessor.setActualGpsData(gpsdata);
		mGpsDCAccessor.releaseNextGpsData();

		//--- Signalisierung
//		CMessage msg(CMessage::Internal_App_Type);
//		msg.setSenderID(GPS_INDEX);
//		msg.setReceiverID(HMI_INDEX);
//		msg.setOpcode(POSITION_DATA);
//		CContext::getMDispContext().getNormalQueue().add(msg, false);

	}
	//beenden, wenn alles gelesen wurde
	DEBUG_PRINT("read done");
	close(mFd);
}
Int32 CGpsFileParserComponent::parseGpsData(GpsData& gpsdata,char* mReadbuffer,
		const UInt32 aSize, const timeval aTimestamp) {
	Int32 retval=-1;

	char *pLineStart = mReadbuffer;
	char *pLineEnd = mReadbuffer ;
	UInt32 lineLength;
	char line[MAX_NMEA_LEN];

	pLineEnd = strchr(pLineStart,'\n');
	while (pLineEnd!=0)
	{
		lineLength = pLineEnd - pLineStart;
		if (lineLength<=MAX_NMEA_LEN)
		{
			memset(line, 0, MAX_NMEA_LEN);
			strncpy(line,pLineStart,lineLength);
		}
		pLineStart = pLineEnd+1;
		pLineEnd = strchr(pLineStart,'\n');
		DEBUG_PRINT("%s",line);
		if(mGpsNMEAParser.parseGPS(line,gpsdata)) {
			retval = 0;
		} else {
			retval = -1;
		}
		//DEBUG_PRINT("%d", retval);
	}

	return retval;
}

void CGpsFileParserComponent::handleMessage(const CMessage& msg){

}
