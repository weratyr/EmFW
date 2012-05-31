#ifndef CGPSFILEPARSER_H_
#define CGPSFILEPARSER_H_

#ifndef CCONTEXT_H
	#include "CContext.h"
#endif

#ifndef ACOMPONENTBASE_H
   #include "AComponentBase.h"
#endif

#ifndef CMESSAGE_H
	#include "CMessage.h"
#endif

#ifndef CGPSDATACONTAINERACCESSOR_H_
	#include "CGpsDataContainerAccessor.h"
#endif

#ifndef CGPSDATACONTAINERADAPTER_H_
	#include "CGpsDataContainerAdapter.h"
#endif

#ifndef CGPSNMEAPARSER_H
	#include "CGpsNMEAParser.h"
#endif

#ifndef GLOBAL_H
	#include "Global.h"
#endif

#include <string>

using std::string;

class CGpsFileParserComponent: public AComponentBase {
public:
	CGpsFileParserComponent(CComponentContext& theContext,char* filename);
	virtual ~CGpsFileParserComponent();
	Int32 parseGpsData(GpsData&, char* ,const UInt32, const timeval);
	virtual void init(void);
	virtual void run(void);
	virtual void handleMessage(const CMessage&);
	CGpsDataContainerAccessor mGpsDCAccessor;
private:
	#define SIMULATION_ACCELERATOR 1
	static const UInt32 GPS_DATA_BUFFER_SIZE=2048;

	Int32 mFd;

	char* mFileName;
	static const char sDelim;
	bool mReadFromFile;
	mode_t mRights;
	CGpsNMEAParser mGpsNMEAParser;
};

#endif /* CGPSFILEPARSER_H_ */
