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
#include "IIOStream.h"

IInputStream::IInputStream(void)
{
   // nothing
}

IInputStream::~IInputStream(void)
{
   // nothing
}

IOutputStream::IOutputStream(void)
{
   // nothing
}

IOutputStream::~IOutputStream(void)
{
   // nothing
}

IIOStream::IIOStream(void)
{
   // nothing
}

IIOStream::~IIOStream(void)
{
   // nothing
}


// Macros to define the operators below

#define SERIALIZEABLE(T) IInputStream& operator>>(IInputStream& in, T& obj) \
		{ cout << "bytes read " << in.read( reinterpret_cast<UInt8*>(&obj), sizeof(T) ) << endl; return in; } \
	IOutputStream& operator<<(IOutputStream& out, const T& obj) \
		{ out.write( reinterpret_cast<const UInt8*>(&obj), sizeof(T) ); return out; }

IInputStream& operator>>(IInputStream& in, CMessage& obj){
	return in;
}

IOutputStream& operator<<(IOutputStream& out, const CMessage& obj){

	return out;
}

/**
 * All basic types are streamable by default:
 */
SERIALIZEABLE( UInt8  );
SERIALIZEABLE( Int8   );

SERIALIZEABLE( UInt16 );
SERIALIZEABLE( Int16  );

SERIALIZEABLE( UInt32 );
SERIALIZEABLE( Int32  );


SERIALIZEABLE( char   );
SERIALIZEABLE( bool   );

SERIALIZEABLE( float  );
SERIALIZEABLE( double );
