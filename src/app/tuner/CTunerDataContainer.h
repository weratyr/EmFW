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

#include "CContainer.h"

// TODO prio 3 :: hier fehlt der CTOR (jowi)
class CTunerDataContainer: public CContainer
{
public:
	// alternativ: Methoden mit Parameter
	bool isPSNameValid(void);
	bool isFreqValid(void);
	bool isPIValid(void);
	const char * getPSName(void);
	const Int32 getFreq(void);
	const Int32 getPI(void);
private:

	// alternative - Method mit Parameter
	void validatePSName(void);
	void validateFreq(void);
	void validatePI(void);
	void setPSName(const char * psName);
	void setFreq(Int32 freq);
	void setPI(Int32 pi);

	// alternativ Flag!
	bool mPSNameValid;
	bool mFreqValid;
	bool mPIValid;
	char mPSName[9];
	Int32 mFreq;
	Int32 mPI;
	CMutex mMutex;
};

