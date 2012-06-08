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
#include "CInetAddr.h"

CInetAddr::CInetAddr()
{

}

CInetAddr::~CInetAddr()
{

}

bool CInetAddr::set(UInt16 port, const char * address)
{
	memset(&mInetAddr, sizeof(mInetAddr),0);
	mInetAddr.sin_family = AF_INET;
	mInetAddr.sin_port = htons(port);

	if (inet_aton(address, &mInetAddr.sin_addr) <= 0)
	{
		return false;
	}

	return true;
}

bool CInetAddr::set(UInt16 port)
{
	memset(&mInetAddr, sizeof(mInetAddr), 0);
	mInetAddr.sin_family = AF_INET;
	mInetAddr.sin_port = htons(port);
	mInetAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	return true;
}
