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
 * Class Implementation of CMiniCommander
 */

#include "CMiniCommander.h"

//Constructor
CMiniCommander::CMiniCommander(const char* device, long lengthLongKeyPressed,
		long threshold, long bitrate) {
	mDevice = new char[strlen(device) + 1];
	strcpy(mDevice, (const char*) device);
	mLengthLongKeyPressed = lengthLongKeyPressed;
	mVolumeThreshold = threshold;
	mBitrate = bitrate;
	mFD = 0;
	volumeChanged = 0;
	init();
	DEBUG_PRINT("read on device: %s with bitrate: %li", device, bitrate);
}

CMiniCommander::~CMiniCommander() {
	stop(); //reset termios-settings and close fds
	delete mDevice;
}

void CMiniCommander::run() {
	DEBUG_PRINT("starting");
	recieve();
}

/**
 * Sets the termios-settings to origin
 */
void CMiniCommander::stop() {
	tcsetattr(mFD, TCSANOW, &mTermios_s);
	close(mFD);
}

/**
 * Initializes the serial interface.
 * returns 1 for success - otherwise 0.
 */
int CMiniCommander::init() {
	DEBUG_PRINT("enter for device [%s] and speed [%lx]", mDevice, mBitrate);

	speed_t speed = mBitrate;
	memset(&mTermios_p, 0x00, sizeof(mTermios_p));
	memset(&mTermios_s, 0x00, sizeof(mTermios_s));

	mFD = open(mDevice, O_RDWR |O_NOCTTY ); //no controlling entity
	if (mFD >0) {
		tcgetattr(mFD, &mTermios_s);
		mTermios_p = mTermios_s;

		// set serial device parameters
		mTermios_p.c_cc[VMIN] = 12; //normally one of our packets contains 12 chars
		mTermios_p.c_cc[VTIME] = 2; //timout 2 sec
		mTermios_p.c_iflag &= ~(ICRNL | INLCR | ISTRIP | IXOFF | IXON);
#ifdef IHFOW  //not defined in POSIX -> QNX
		mTermios_p.c_cflag &= ~ IHFLOW;
#endif
#ifdef OHFLOW //not defined in POSIX -> QNX
		mTermios_p.c_cflag &= ~ OHFLOW;
#endif
#ifdef CRTSCTS //not defined in POSIX ->LINUX
		mTermios_p.c_cflag &= ~CRTSCTS;
#endif
		mTermios_p.c_cflag &= ~(PARENB);
		mTermios_p.c_lflag &= ~(ECHO | ICANON | ECHOE | ECHOK | ECHONL | IEXTEN);
		mTermios_p.c_oflag &= ~(OPOST);
				// baud, 8N1
		mTermios_p.c_cflag |= (CREAD);
		mTermios_p.c_cflag |= (CS8 | CSIZE);
		cfsetispeed(&mTermios_p, speed);
		cfsetospeed(&mTermios_p, speed);
		tcsetattr(mFD, TCSADRAIN, &mTermios_p);
	} else {
		DEBUG_PRINT("Error opening Device [%s]", mDevice);
		return 0;
	}
	return 1;
}

/**
 * Sends a high level key event using the component messaging system
 * addressing the HMI as receiver.
 */
void CMiniCommander::sendKey(CommanderKeyCode keyValue) {

	DEBUG_PRINT("value = %d", keyValue);

	/*
	 * TODO
	 * create a message-object from CMessage,
	 * send this with the given keyValue to the component
	 * which should show a reaction of the keyValue
	 */
}

//TODO prio 3 :: errorhandling if no stx/etx is received or more/less than 12 bytes are received
void CMiniCommander::recieve() {
	DEBUG_PRINT("Enter")
		char data[12]; //normally one of our packets contains 12 chars
		int numChars = 0;
		char stxCount = 0;
		char etxCount = 0;
		int i;

		mRecBlock.reset();

		DEBUG_PRINT("Start reading")
		while (1) {

			numChars=read(mFD, &data,12);////normally one of our packets contains 12 chars
			if(numChars==-1)
			{
				printf("Error on reading from miniCom-dev");
				return;
			}
			DEBUG_PRINT("read package with numChars %d",numChars);
			for(i=0;i<numChars; i++)
			{
				if (data[i] == STX) // Count Start/ENd packts to ensure we have complete packets
					stxCount++;
				if (data[i] == ETX) {
					etxCount++;
					if (stxCount < etxCount) {
						DEBUG_PRINT("Bad Packet!")
						// We have a packet without start. Probably we did not
						// receive the beginning of the packet
						// Throw it away
						mRecBlock.idxRead = mRecBlock.idxWrite + 1;
					}
				}
				mRecBlock.add(data[i]); // add data to receive block
			}

			if (etxCount > 0) {
				if (stxCount > 0) {
					DEBUG_PRINT("Start interpret telegram")
					interpretTelegram();
					etxCount--;
					stxCount--;
				} else {
					// We have half a packet. Throw it away
				}
			}
		}
		DEBUG_PRINT("Leave");
}

/**
 *  Interpret received telegrams
 */
void CMiniCommander::interpretTelegram() {
	short numChars; // number of characters in receive buffer
	CMiniCommanderDataBlock telegram;
	char *ptr;
	char wheelCount;

	ptr = (char*) &telegram;

	// determine number of characters
	numChars = mRecBlock.count(); // number of characters in the receive buffer


	// if pointer on different ends, then adjust by use of buffer length
	if (numChars < 0) {
		numChars += MCMD_BUF_LEN;
	}

	// validate  size of characters against data block structure
	if ((unsigned short)numChars < sizeof(CMiniCommanderDataBlock)) {
		// There is no complete packet in the receive buffer.
		printf("Error imcomplete buffer in InterpretTelegem\n");
		return;
	}

	// Copy to local buffer
	for (unsigned int i = 0; i < sizeof(CMiniCommanderDataBlock); i++) {
		ptr[i] = mRecBlock.getByte();
	}

	DEBUG_PRINT("Interpreting Telegram STX=%2.0X LEN=%2x%2X SRC=%2x%2x CMD=%2X%2X DATA=%2X%2X CS=%2X%2X ETX=%2X",
			telegram.stx, telegram.lengthH, telegram.lengthL,
			telegram.sourceH, telegram.sourceL, telegram.cmdH,
			telegram.cmdL, telegram.dataH, telegram.dataL, telegram.csH,
			telegram.csL, telegram.etx);

	switch (telegram.cmdH) {
	case 0x35: // Key Scan or Commander present
		if (telegram.cmdL == 0x35) // Key Scan
		{
			// Engineering Mode
			if ((telegram.dataL & 0x06) == 0x06) // Key C+ E pressed
				key35State = getTimestamp();
			else if (key35State != 0) {
				sendKey(MC_SOFT_CE_PRESSED); //F13
				keyCState = 0;
				keyEState = 0;
				key35State = 0;
			}

			if (telegram.dataH & 0x02) // Key A pressed
			{
				keyAState = getTimestamp();
			} else if (keyAState != 0) {
				if (getTimestamp() - keyAState < mLengthLongKeyPressed)
					sendKey(MC_SOFT_A_SHORT); //F1
				else
					sendKey(MC_SOFT_A_LONG); //F7
				keyAState = 0;
			}

			if (telegram.dataL & 0x08) // Key B pressed
			{
				keyBState = getTimestamp();
			} else if (keyBState != 0) {
				if (getTimestamp() - keyBState < mLengthLongKeyPressed)
					sendKey(MC_SOFT_B_SHORT); //F2
				else
					sendKey(MC_SOFT_B_LONG); //F8
				keyBState = 0;
			}

			if (telegram.dataL & 0x02) // Key C pressed
				keyCState = getTimestamp();
			else if (keyCState != 0) {
				if (getTimestamp() - keyCState < mLengthLongKeyPressed)
					sendKey(MC_SOFT_C_SHORT); //F3
				else
					sendKey(MC_SOFT_C_LONG); //F9
				keyCState = 0;
			}

			if (telegram.dataL & 0x01) // Key D pressed
				keyDState = getTimestamp();
			else if (keyDState != 0) {
				if (getTimestamp() - keyDState < mLengthLongKeyPressed)
					sendKey(MC_SOFT_D_SHORT); //F4
				else
					sendKey(MC_SOFT_D_LONG); //F10
				keyDState = 0;
			}

			if (telegram.dataL & 0x04) // Key E pressed
				keyEState = getTimestamp();
			else if (keyEState != 0) {
				if (getTimestamp() - keyEState < mLengthLongKeyPressed)
					sendKey(MC_SOFT_E_SHORT); //F5
				else
					sendKey(MC_SOFT_E_LONG); //F11
				keyEState = 0;
			}

			if (telegram.dataH & 0x01) // Key F pressed
				keyFState = getTimestamp();
			else if (keyFState != 0) {
				if (getTimestamp() - keyFState < mLengthLongKeyPressed)
					sendKey(MC_SOFT_F_SHORT); //F6
				else
					sendKey(MC_SOFT_F_LONG); //F12
				keyFState = 0;
			}

			if (telegram.dataH & 0x04) // Wheel pressed
			{
				keyIncrState = getTimestamp();
				volumeThreshold = mVolumeThreshold;
			} else if (keyIncrState != 0) {
				if (volumeChanged == 0)
					if (getTimestamp() - keyIncrState < mLengthLongKeyPressed)
						sendKey(MC_SOFT_ROTARY_SHORT); // Enter
					else
						sendKey(MC_SOFT_ROTARY_LONG); //ESC
				else
					volumeChanged = 0; // on release rest flag

				keyIncrState = 0;
			}

		}
		if (telegram.cmdL == 0x41) {
			// Commander present
			DEBUG_PRINT("Commander Present\n");
		}

		break;
	case 0x41: // Wheel left
		wheelCount = asciiToHex(telegram.dataL, telegram.dataH);
		if (keyIncrState != 0) // Wheel pressed
		{
			volumeThreshold -= wheelCount;
			if (volumeThreshold <= 0) {
				volumeChanged = 1;
				while (wheelCount) {
					sendKey(MC_SOFT_ROTARY_LEFT_PRESSED);
					wheelCount--;
				}
			}
		} else {
			while (wheelCount) {
				sendKey(MC_SOFT_ROTARY_LEFT_UNPRESSED);
				wheelCount--;
			}
		}
		break;
	case 0x39: // Wheel right
		wheelCount = asciiToHex(telegram.dataL, telegram.dataH);
		if (keyIncrState != 0) // Wheel pressed
		{
			volumeThreshold -= wheelCount;
			if (volumeThreshold <= 0) {
				volumeChanged = 1;
				while (wheelCount) {
					sendKey(MC_SOFT_ROTARY_RIGHT_PRESSED);
					wheelCount--;
				}
			}
		} else {
			while (wheelCount) {
				sendKey(MC_SOFT_ROTARY_RIGHT_UNPRESSED);
				wheelCount--;
			}
		}
		break;
	default: // Unknown command
		DEBUG_PRINT("Unknown command received")
		break;

	}
}

/**
 * Generate timestamp using milliseconds
 */
unsigned long CMiniCommander::getTimestamp() {
	struct timespec tm;
	static int ssec = 1;
	static int snsec = -1;
	clock_gettime(CLOCK_REALTIME, &tm);

	if (ssec == -1) {
		ssec = tm.tv_sec;
		snsec = tm.tv_nsec;
		return 0;
	} else {
		return (tm.tv_sec - ssec) * 1000 + (tm.tv_nsec - snsec) / 1000000;
	}
}

/**
 * Convert two ASCII characters to a hex value
 */
unsigned char CMiniCommander::asciiToHex(unsigned char lowNibble,
		unsigned char highNibble) {
	unsigned char data;

	data = lowNibble - 0x30;
	data += (highNibble - 0x30) * 16;

	return (data);
}
