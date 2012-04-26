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

#ifndef H_EVENTS
#define H_EVENTS

enum
{

	//allgemein
	TICK_SERVICE_TYPE,
	REQUEST_TYPE,
	UPDATE_TYPE,
	DONT_CARE_TYPE,
	ADMIN_TYPE,
	INFOMSG_TYPE,
	LOCK_TYPE,
	UNLOCK_TYPE,


	//TUNER
	GET_CURRENT_STATION, //Anfrage nach dem aktuellen Sender
	GET_CURRENT_FREQUENCY, // Anfrage nach der aktuellen Frequenz
	GET_NEXT_FREQUENCY, //Anfrage nach der naechsten, besten Frequenz
	GET_PREV_FREQUENCY, //Anfrage nach der vorherigen, besten Frequenz
	FREQUENCY,
	SET_FM,
	SET_AM,
	SET_UKW,
	SET_MW,
	SET_LW,

	//TUNER an HMI
	RDS_INFO, //Eine neue RDS Nachricht steht zur Verfuegung
	NEW_FREQUENCY, //Die neue Frequenz des gleichen Senders [bei schlechtem Empfang]
	FREQUENCY_INFO, // Frequenz empfangen
	RDS_ONOFF_INFO, // RDS aktiviert oder nicht...
	RADIOTEXT,

	// Navi an HMI
	NAVI_NEW_MAP,

	// HMI -> Navi
	SET_NEXT_DESTINATION_Navi, // neues Ziel gesetzt
	STOP_GUIDING_Navi,
	CONTINUE_GUIDING_Navi,

	//CDPlayer
	PLAY, //Spielt den gewaehlten Track der aktuellen Liste
	PAUSE, //Pausiert den CD Player
	CONTINUE, //Startet den CD Player wieder
	EJECT, //S/W Eject
	DECKSTATUS,
	TRACKPOSITION,
	AUDIODISCINFO,
	GET_CURRENT_TRACK, //Anfrage nach dem aktuellen Track
	STATUS_CURRENT_LIST,//Antwort auf Anfrage nach der aktuellen Track Liste
	GET_TRACK_LIST, //Anfrage nach der vollstaendigen Track Liste
	SET_TRACK_LIST, //Schickt eine Liste zum speichern/abspielen
	REPEAT, //Soll die aktuelle Liste im Repeat Modus abgespielt werden?
	RANDOM, //Soll die aktuelle Liste im Shuffle Modus abgespielt werden?
	SCAN, //Soll die aktuelle Liste im Scan Modus abgespielt werden?
	STOP, //Stop my CD

	NO_DISK, //Es befindet sich keine CD im CD Player
	NEW_TRACK,
//Es wird ein neuer Track gespielt

	// GPS Komponente
	POSITION_DATA
};

/*
 * Beachte: (betrifft IcmKeyCode und Commander KeyCode)
 *
 * Bei Verwendung von Tastatur und MaxiCom sind die Enum-Werte
 * von IcmKeyCode und CommanderKeyCode entsprechend anzugleichen.
 */

enum IcmKeyCode
{
	//Tastatur
	// Beachte: für Weiter-Entwicklung Abhängikeiten zu ASCII auflösen auf MaxiCom-Keys mappen
	CD_KEY = 'c', //99
	TUNER_KEY = 't' //116
};

enum CommanderKeyCode
{
	//MaxiCom
	/*
	 * Beachte: für Weiter-Entwicklungen bei MaxiCom-Hard-Keys Abhängikeiten zu ASCII-Tastatur-Eingaben auflösen
	 * 			und den entsprechenden Dezimal-Werten aufsteigend sotieren
	 */
	//RELEASED
	CDTV_RELEASED = CD_KEY,
	RADIO_RELEASED = TUNER_KEY,
	NAME_RELEASED,
	TEL_RELEASED,
	NAV_RELEASED,
	INFO_RELEASED,
	CAR_RELEASED,
	SETUP_RELEASED,
	LEFT_RELEASED,
	RIGHT_RELEASED,
	RETURN_RELEASED,
	SOFT_A_RELEASED,
	SOFT_B_RELEASED,
	SOFT_C_RELEASED,
	SOFT_D_RELEASED,
	SMALL_WHEEL_RELEASED,
	BIG_WHEEL_RELEASED,
	//PRESSED
	RADIO_PRESSED,
	CDTV_PRESSED,
	NAME_PRESSED,
	TEL_PRESSED,
	NAV_PRESSED,
	INFO_PRESSED,
	CAR_PRESSED,
	SETUP_PRESSED,
	LEFT_PRESSED,
	RIGHT_PRESSED,
	RETURN_PRESSED,
	SOFT_A_PRESSED,
	SOFT_B_PRESSED,
	SOFT_C_PRESSED,
	SOFT_D_PRESSED,
	SMALL_WHEEL_PRESSED,
	BIG_WHEEL_PRESSED,
	//TURNED
	SMALL_WHEEL_LEFT_TURNED,
	BIG_WHEEL_LEFT_TURNED,
	SMALL_WHEEL_RIGHT_TURNED,
	BIG_WHEEL_RIGHT_TURNED,
	//PRESSED-TURNED
	SMALL_WHEEL_LEFT_PRESSED_TURNED,
	BIG_WHEEL_LEFT_PRESSED_TURNED,
	SMALL_WHEEL_RIGHT_PRESSED_TURNED,
	BIG_WHEEL_RIGHT_PRESSED_TURNED,

	KEY_ILLUM_REQUEST_MaxiCom,



	/*
	 * MiniCommander Soft-Key-Events,
	 * beginnend mit Enum-Wert=300, da Ascii-Zeichen kleiner 300
	 */
	MC_SOFT_CE_PRESSED	=300,
	MC_SOFT_A_SHORT,
	MC_SOFT_A_LONG,
	MC_SOFT_B_SHORT,
	MC_SOFT_B_LONG,
	MC_SOFT_C_SHORT,
	MC_SOFT_C_LONG,
	MC_SOFT_D_SHORT,
	MC_SOFT_D_LONG,
	MC_SOFT_E_SHORT,
	MC_SOFT_E_LONG,
	MC_SOFT_F_SHORT,
	MC_SOFT_F_LONG,
	MC_SOFT_ROTARY_SHORT,
	MC_SOFT_ROTARY_LONG,
	MC_SOFT_ROTARY_RIGHT_UNPRESSED,
	MC_SOFT_ROTARY_LEFT_UNPRESSED,
	MC_SOFT_ROTARY_RIGHT_PRESSED,
	MC_SOFT_ROTARY_LEFT_PRESSED

};

#endif // H_Events
