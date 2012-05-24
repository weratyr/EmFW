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
 * Class Declaration for CMiniCommander
 *
 * Encapsulates the access to the serial-interface
 * receives and interprets the telegrams from the
 * Minicommander-Device
 *
 */

#ifndef _CMINICOMMANDER_H
#define _CMINICOMMANDER_H

#ifndef H_EVENTS
	#include "Events.h"
#endif
#ifndef _CMINICOMMANDERRECORDBLOCK_H
	#include "CMiniCommanderRecordBlock.h"
#endif
#ifndef _LOWLEVEL_EVENTS_MINICOM_H
	#include "LowLevelEventsMiniCom.h"
#endif
#ifndef _CMINICOMMANDERDATABLOCK_H
	#include "CMiniCommanderDataBlock.h"
#endif
#ifndef _CMESSAGE_H
	#include "CMessage.h"
#endif
#ifndef _CCONTEXT_H
	#include "CContext.h"
#endif
#ifndef _CTHREAD_H
	#include "CThread.h"
#endif

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <termios.h>

#ifdef __QNX__
	#define MC_DEVICE "/dev/ser1"
#else
	#define MC_DEVICE "/dev/ttyS0"
#endif

#define MCMD_BUF_LEN   100
#define MAX_KEYSENDER  10

// Commands
#define KEY_SCAN_RESULT       0x55
#define WHEEL_RIGHT           0x99
#define WHEEL_LEFT            0xaa
#define COMMANDER_PRESENT     0x5a
#define STX                   0x02
#define ETX                   0x0d
#define SOURCE                0xf0

class CMiniCommander {
public:
	CMiniCommander(const char* device = MC_DEVICE,
			long lengthLongKeyPressed = 500, long threshold = 2, long bitrate =	B57600);
	~CMiniCommander();

	void run();
	int init();
	void stop();

private:
	char *mDevice;				// name of the serial device
	unsigned long mLengthLongKeyPressed;
	long mVolumeThreshold;
	long mBitrate;				// bitrate used during intialization
	int mFD;					// file descriptor for the serial interface
	CMiniCommanderRecordBlock mRecBlock;

	unsigned long int keyAState;
	unsigned long int keyBState;
	unsigned long int keyCState;
	unsigned long int keyDState;
	unsigned long int keyEState;
	unsigned long int keyFState;
	unsigned long int keyIncrState;
	unsigned long int key35State;
	int volumeChanged;
	int volumeThreshold;

	struct termios mTermios_s; //saves old termios-settings
	struct termios mTermios_p; //for current termios-settings

	void sendKey(CommanderKeyCode keyValue);
	void recieve();
	void interpretTelegram(); // interpret received telegrams

	static unsigned long getTimestamp();
	static unsigned char asciiToHex(unsigned char lowNibble, unsigned char highNibble);
};

#endif

