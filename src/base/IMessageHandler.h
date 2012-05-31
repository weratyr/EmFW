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

#ifndef _IMESSAGEHANDLER_H
#define _IMESSAGEHANDLER_H

// Forward declaration

class CMessage;

/**
 * Interface for all message handler. A message handler is responsible
 * to handle the message received correctly.
 */
class IMessageHandler
{
 public:
 	/**
 	 * Virtuelle-VerarbeitungsRoutine fuer die Events in der Queue
 	 * \param msg ist die Nachricht (Message), die Verarbeitet werden soll.
 	 */
   virtual void handleMessage(const CMessage& msg) = 0;
};

#endif // end of _IMESSAGEHANDLER


