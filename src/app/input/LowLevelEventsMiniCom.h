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
 * Definition of low level events triggered by the minicom device
 */

#ifndef _LOWLEVEL_EVENTS_MINICOM_H
#define _LOWLEVEL_EVENTS_MINICOM_H

#define DEVICE_ID						0x0002	//we are a non most device
#define FBLOCK_ID 						0xF0	//suppl. spec. FBLOCK_ID
#define INSTANCE_ID						0x01
#define FUNCTION_ID_BUTTON_SHORT		0x0F00
#define FUNCTION_ID_BUTTON_LONG			0x0F01
#define FUNCTION_ID_WHEEL				0x0F02
#define FUNCTION_ID_WHEEL_PUSHED		0x0F03
#define FUNCTION_ID_WHEEL_TURN_PUSHED	0x0F04

#define OP_TYPE_GET         			1 //get

#define DATA_BUTTON_A					0x41 //ASCII-Code for A
#define DATA_BUTTON_B					0x42 //ASCII-Code for B
#define DATA_BUTTON_C					0x43 //ASCII-Code for C
#define DATA_BUTTON_D					0x44 //ASCII-Code for D
#define DATA_BUTTON_E					0x45 //ASCII-Code for E
#define DATA_BUTTON_F					0x46 //ASCII-Code for F
#define DATA_WHEEL_DIR_RIGHT			0x02
#define DATA_WHEEL_DIR_LEFT				0x04
#define DATA_WHEEL_PRESSED_SHORT		0x08
#define DATA_WHEEL_PRESSED_LONG			0x10

#endif
