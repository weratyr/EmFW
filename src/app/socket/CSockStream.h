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
#ifndef _CSOCKSTREAM_H_
#define _CSOCKSTREAM_H_

#ifndef _CINETADDR_H_
	#include "CInetAddr.h"
#endif

#ifndef _GLOBAL_H
	#include "Global.h"
#endif

#ifndef _IIOSTREAM_H_
	#include "IIOStream.h"
#endif

#ifndef _CMESSAGE_H
	#include "CMessage.h"
#endif

#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>

/**
 * Wrapper for Cocket-Connection
 */
class CSockStream  : public IIOStream
{
	friend class CSockConnector;
	friend class CSockAcceptor;
public:
	CSockStream();
	~CSockStream();

	/**
	 * read size bytes from stream into buffer
	 * @return number of bytes actually read
	 */
   	virtual const Int32 read ( UInt8 * bufferPtr, const UInt32 size );

   	/*
   	 * read CMessages from stream into buffer
   	 * @return number of bytes actually read
   	 */
   	virtual const Int32 read ( CMessage* const messageBufferPtr, const UInt32 size );

   	/**
	 * send size bytes from the buffer into stream
	 * @return number of bytes actually send
	 */
	virtual const Int32 write( const UInt8* const bufferPtr, const UInt32 size ) ;

	/**
	 * send CMessages into the stream
	 * @return number of bytes actually sent
	 */
	virtual const Int32 write( const CMessage* const messageBufferPtr, const UInt32 size);

	virtual void flush( void );

	/**
	 * close this stream for reading and writing
	 */
	void close();
private:
	int mSocket;
};

#endif //_CSOCKSTREAM_H_
