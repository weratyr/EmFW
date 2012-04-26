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

/**
 * Class declaration for the CMiniCommanderRecordBlock
 *
 * Describes a Telegram sent by Minicommander device.
 */

#ifndef _CMINICOMMANDERRECORDBLOCK_H
#define _CMINICOMMANDERRECORDBLOCK_H

#define MCMD_BUF_LEN 100

class CMiniCommanderRecordBlock {
public:
	unsigned char buffer[MCMD_BUF_LEN]; // receive buffer for chars from front controller
	unsigned short idxWrite; // index of the next writable char
	unsigned short idxRead; // index of the next readable char
	unsigned char telegram[5]; // net-data composed by the telegram decoder

	void reset() {
		idxWrite = 0;
		idxRead = 0;
	}

	void add(char data) {
		buffer[idxWrite] = data;
		++idxWrite;
		if (idxWrite >= MCMD_BUF_LEN) {
			idxWrite = 0; // Wrap arround in ring buffer
		}
	}

	short count() {
		return idxWrite - idxRead;
	}

	unsigned char getByte() {
		unsigned char data;

		data = buffer[idxRead]; // get character
		idxRead++; // next position
		if (idxRead >= MCMD_BUF_LEN) { // go to start of ring buffer
			idxRead = 0;
		}

		return data;
	}
};

#endif
