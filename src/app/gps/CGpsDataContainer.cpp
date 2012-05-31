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

CGpsDataContainer::CGpsDataContainer() :
mGpsReadSemaphore(false, true),mGpsWriteSemaphore(false, true)
{
	init();
}

CGpsDataContainer::~CGpsDataContainer() {
}

void CGpsDataContainer::init(){
	DEBUG_PRINT("entered");
	mGpsWriteSemaphore.give();	// Ersten Wert schreiben lassen
	mGpsReadSemaphore.give();	// "Vergessen"
}

/*
 * TODO:
 * GPS Daten setzen
 * Sicherstellen, dass nur wenn die GPS Daten abgeholt wurden neue Werte gesetzt werden
 */

void CGpsDataContainer::setGpsData(const GpsData& gpsdata){
	//DEBUG_PRINT("--- Take Write Sempahore --- SET");
	mGpsWriteSemaphore.take(true);
	//DEBUG_PRINT("--- Take Read Sempahore --- SET");
	mGpsReadSemaphore.take(true);
	//DEBUG_PRINT("--- Memcopy SET GPS DATA");
	memcpy(&mGpsData, &gpsdata,sizeof(GpsData));
	//DEBUG_PRINT("--- Give Read Sempahore --- SET");
	//mGpsReadSemaphore.give();

}

/*
 * TODO:
 * GPS Daten holen
 * Sicherstellen, dass nur wenn die GPS Daten abgeholt wurden neue Werte gesetzt werden
 */
void CGpsDataContainer::getGpsData(GpsData& gpsdata){
	//DEBUG_PRINT("--- Take Read Sempahore --- GET");
	mGpsReadSemaphore.take(true);
	memcpy(&gpsdata, &mGpsData,sizeof(GpsData));
	//DEBUG_PRINT("--- Give Write Sempahore --- GET");
	mGpsWriteSemaphore.give();
	//DEBUG_PRINT("--- Give Read Sempahore --- GET");
	mGpsReadSemaphore.give();
}

/*
 * TODO:
 * Nächsten GPS Wert zum Lesen freigeben
 */

void CGpsDataContainer::releaseNextGpsData() {
	//DEBUG_PRINT("--- Give Read Sempahore --- REL");
	mGpsReadSemaphore.give();
}
