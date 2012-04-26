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
 * Class declaration for the CMiniCommanderDataBlock
 *
 * Describes a Telegram sent by Minicommander device.
 */

#ifndef _CMINICOMMANDERDATABLOCK_H
#define _CMINICOMMANDERDATABLOCK_H

class CMiniCommanderDataBlock {
public:
	char stx; // Start Block ID (0x02)
	char lengthH; // Length of Block after stx (high nibble)
	char lengthL; // Length of Block after stx (low nibble)
	char sourceH; // Source ID (high nibble)
	char sourceL; // Source ID (low nibble)
	char cmdH; // Command/DataType (high nibble)
	char cmdL; // Command/DataType (low nibble)
	char dataH; // Data (high nibble)
	char dataL; // Data (low nibble)
	char csH; // Block Checksumme (high nibble)
	char csL; // Block Checksumme (low nibble)
	char etx; // End Block ID (0x0D)
};

#endif

