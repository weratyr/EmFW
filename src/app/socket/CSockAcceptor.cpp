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
#include "CSockAcceptor.h"
//#include <sys/socket.h>

CSockAcceptor::CSockAcceptor()
{

}

/**
 * cleans up the socket, if this winsock is used
 */
CSockAcceptor::~CSockAcceptor()
{

}

bool CSockAcceptor::open(CInetAddr& address)
{
	if ((mListenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		return false;
	}

	int optValue = 1;
	setsockopt(mListenfd,SOL_SOCKET,SO_REUSEADDR,(char *) &optValue,sizeof(int) );
	bind( mListenfd, (sockaddr*) &address.mInetAddr, sizeof( address.mInetAddr ) );
	listen( mListenfd, 5 );
	return true;
}

bool CSockAcceptor::accept(CSockStream& stream, Int8 timeout)
{
	/* If timeout is equal 0 we will do a blocking accept and wait until we get a connection. Otherwise timeout will be the time
	* in seconds we will wait for a new connection. If there is no  connection request we return false, otherwise we accept the
	* connection request and return the result of accept.
	*/
	if (timeout > 0)
	{
		fd_set rfds;
		struct timeval tv;
		int retval;

		FD_ZERO( &rfds );
		FD_SET( (unsigned int)mListenfd, &rfds );
		tv.tv_sec = timeout;
		tv.tv_usec = 0;

		retval = select(mListenfd + 1, &rfds, NULL, NULL, &tv);
		if ( !retval )
		{
			// Timeout with no connection request.
			return false;
		}
    }

	/* When we are here we either have no timeout (so we will do a blocking call to 'accept') or there is a pending connection request and our
	* call to 'accept' will return the new connection.
	*/
	int connfd = ::accept(mListenfd, (sockaddr*) NULL,  NULL);

	if ( connfd == -1 )
	{
		return false;
	}

	stream.mSocket = connfd;
	return true;
}
