/***************************************************************************
 *  openICM-framework
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

#ifndef _GLOBAL_H
#define _GLOBAL_H
/**
 *
 * This file contains the common elementar data types used in the
 * project. Using Int8, Int16 etc. help easiliy porting the codes.
 * This file does not contain any specific hardware pins or ports
 */

/**
 * enable for printing out a lot of debug messages. When running on target system, it could be
 * possible, that the MainDispatcher stops his work.
 */
#include <stdio.h>
/* Makro fuer Debugausgaben: */
#ifdef DEBUG_MODE

	//TODO prio 2 :: weird output via qconn during binding of threads to cpu (multiple entries) (ak)

	#define DEBUG_PRINT(format, args...)\
			{\
				static char message[512];\
				snprintf(message,512,format, ##args);\
				fprintf(stdout, "[%s] : %s\n", __PRETTY_FUNCTION__, message);\
				fflush(stdout);\
			}
#else
	#define DEBUG_PRINT(format, args...)
#endif // DEBUG_MODE

// synonyme for elementar data types
typedef signed char        Int8;
typedef unsigned char      UInt8;
typedef signed short       Int16;
typedef unsigned short     UInt16;
typedef signed int         Int32;
typedef unsigned int       UInt32;

// Definition fuer nullpointer und null character
#define NULLPTR         0
#define NULLCHAR        0

// simple helpers to release dynamic allocated memory
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }

// Macro used to round up the memory required due to the alignment
#define MAKE_ALIGNMENT_SIZE(size)  ((( (size) + (sizeof(Int32) - 1) ) \
                                   / sizeof(Int32))*sizeof(Int32))

// Macros for converting 2 bytes to a word (16 bits)
//  the following macros work for Little and Big-Endian
// Syntax MAKE_UINT16(FirstByte, SecondByte)
#define MAKE_INT16(a,b)	   Int16(  ((UInt16) a << 8) | UInt8(b) )
#define MAKE_UINT16(a,b)   UInt16( ((UInt16) a << 8) | UInt8(b) )

// Syntax LOW_BYTE(16 Bit-Word), HIGH_BYTE(16 Bit-Word)
#define LOW_BYTE(w)        UInt8(w&0xFF)
#define HIGH_BYTE(w)       UInt8(w >> 8)

/*
 * this defines maximum size of an OpenGL-Window,
 * values actually from Qnx-target D1
 */
#define WINDOW_WIDTH	800 // [pixel]
#define WINDOW_HEIGHT	480 // [pixel]

#endif  // _GLOBAL_H


