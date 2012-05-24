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

#include "CHMIGUIImageDesc.h"

const int CHMIGUIImageDesc::mBitsForPixelType[] = { 8, 		// UNSIGNED_BYTE
										8, 		// BYTE
										1, 		// BITMAP
										2*8, 	// SHORT
										2*8, 	// UNSIGNED_SHORT
										4*8, 	// INT
										4*8, 	// UNSIGNED_INT
										4*8 }; 	// FLOAT
/**
 * components of the PixelFormat
 */
const int CHMIGUIImageDesc::mPixelFormatComponents[] = { 4, // RGBA
									3, // RGB
									1, // ALPHA
									1, // RED
									1, // GREEN
									1, // BLUE
									1, // ALPHA_BIAS
									1, // LUMINANCE
									2}; // LUMINANCE_ALPHA
