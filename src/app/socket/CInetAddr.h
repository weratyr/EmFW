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
#ifndef _CINETADDR_H_
#define _CINETADDR_H_

#ifndef _GLOBAL_H
	#include "Global.h"
#endif

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string.h>
using namespace std;

class CInetAddr
{
	friend class CSockConnector;
	friend class CSockAcceptor;
public:
	CInetAddr();
	~CInetAddr();

	/**
	 * set address represented by this object
	 * \param port port to be used
	 * \param address address to be used
	 * \return true on succes
	 */
	bool set(UInt16 port, const char * address);

	/**
	 * \param port port to be used, address is set to ANY
	 * \return true if success
	 */
	bool set(UInt16 port);
private:
	sockaddr_in mInetAddr;
};

#endif //_CINETADDR_H_
