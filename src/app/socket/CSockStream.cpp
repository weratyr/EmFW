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
#include "CSockStream.h"

CSockStream::CSockStream() : mSocket(0)
{
	/**
	* We need to ignore this signal, otherwise we won't receive and error from <code> send </code> when the
	* connection is closed but instead the process would be terminated.
	*/
	signal( SIGPIPE, SIG_IGN );
}

CSockStream::~CSockStream() {}

const Int32 CSockStream::read(UInt8 * bufferPtr, const UInt32 size)
{
	return ::read( mSocket, bufferPtr, size );
}

const Int32 CSockStream::write(const UInt8* const bufferPtr, const UInt32 size)
{
	return ::write( mSocket, bufferPtr, size );
}

const Int32 CSockStream::write(const CMessage* const messageBufferPtr, const UInt32 size)
{
	return ::write(mSocket, messageBufferPtr, size);
}

const Int32 CSockStream::read ( CMessage* messageBufferPtr, const UInt32 size )
{
	return ::read(mSocket, messageBufferPtr, size);
}

void CSockStream::flush(void)
{
	// todo
}

void CSockStream::close()
{
	::close(mSocket);
	mSocket = 0;
}
