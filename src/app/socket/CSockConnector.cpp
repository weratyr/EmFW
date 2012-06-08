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
#include "CSockConnector.h"

CSockConnector::CSockConnector()
{

}

/**
 * \brief used to cleanup the windows socket(s)
 */
CSockConnector::~CSockConnector()
{

}

bool CSockConnector::connect(CSockStream& stream, CInetAddr& address, bool block)
{
	int sockfd;

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0 )) < 0)
	{
		return false;
	}

	if (::connect(sockfd, (sockaddr*) &address.mInetAddr, sizeof(address.mInetAddr)) < 0)
	{
		return false;
	}
	// save the socket filedescriptor
	stream.mSocket = sockfd;
	return true;
}
