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

#ifndef _IIOSTREAM_H_
#define _IIOSTREAM_H_

#ifndef _GLOBAL_H
	#include "Global.h"
#endif

#ifndef _CMESSAGE_H
	#include "CMessage.h"
#endif

#include <iostream>
using namespace std;

/**
 * Interface for Input-Stream
 */
class IInputStream
{
public:
	virtual ~IInputStream( void );
    virtual const Int32 read ( UInt8 * bufferPtr, const UInt32 size ) = 0;
protected:
	IInputStream( void );
};

/**
 * Interface for output-stream
 */
class IOutputStream
{
public:
	virtual ~IOutputStream( void );
	virtual const Int32 write( const UInt8* const bufferPtr, const UInt32 size ) = 0;
	virtual void flush( void ) = 0;
protected:
	IOutputStream( void );
};

/**
 * Stream for Input-Output
 */
class IIOStream: public IInputStream, public IOutputStream
{
public:
	virtual ~IIOStream(void);
protected:
	IIOStream(void);
};

/**
 * operators for streaming
 */
IInputStream& operator>>(IInputStream& in, UInt8& obj);
IInputStream& operator>>(IInputStream& in, UInt16& obj);
IInputStream& operator>>(IInputStream& in, UInt32& obj);
IInputStream& operator>>(IInputStream& in, Int8& obj);
IInputStream& operator>>(IInputStream& in, Int16& obj);
IInputStream& operator>>(IInputStream& in, Int32& obj);
IInputStream& operator>>(IInputStream& in, char& obj);
IInputStream& operator>>(IInputStream& in, bool& obj);
IInputStream& operator>>(IInputStream& in, float& obj);
IInputStream& operator>>(IInputStream& in, double& obj);

IOutputStream& operator<<(IOutputStream& out, const UInt8& obj);
IOutputStream& operator<<(IOutputStream& out, const UInt16& obj);
IOutputStream& operator<<(IOutputStream& out, const UInt32& obj);
IOutputStream& operator<<(IOutputStream& out, const Int8& obj);
IOutputStream& operator<<(IOutputStream& out, const Int16& obj);
IOutputStream& operator<<(IOutputStream& out, const Int32& obj);
IOutputStream& operator<<(IOutputStream& out, const char& obj);
IOutputStream& operator<<(IOutputStream& out, const bool& obj);
IOutputStream& operator<<(IOutputStream& out, const float& obj);
IOutputStream& operator<<(IOutputStream& out, const double& obj);

/**
 * the following macros are used to serialize and deserialize enumerations
 * Normally an enum has 4 bytes. Most enumerations need only one or two bytes
 * such that a reinterpretation can save the memory
 */
#define STREAMABLE_ENUM(T , U ) \
inline IInputStream& operator>>(IInputStream& in, T& obj) \
{  return in >> reinterpret_cast<U&>(obj); } \
inline IOutputStream& operator<<(IOutputStream& out, const T& obj) \
{  return out << reinterpret_cast<const U&>(obj); }
#endif //_IIOSTREAM_H_

