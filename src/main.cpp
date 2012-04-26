/***************************************************************************
 *  openICM-application
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

#ifndef _CADMINCOMPONENT_H
	#include "CAdminComponent.h"
#endif

//TODO prio 1 :: fill file 'README' with coding rules

int main(int argc, char* argv[])
{
	DEBUG_PRINT("starting up");
	CContext::createContexts();

	// starting every component as processes. creation as threads is
	// supported and may be interesting for future applications
	CAdminComponent admin(CContext::getAdminContext());
	CThread CAdmin_thread(admin,
			CContext::getAdminContext().getContextNamePtr(),
			CContext::ADMIN_STACK_SIZE, CContext::ADMIN_PRIORITY,
			CContext::ADMIN_AFFINITY, true);

	// now proceed within admin component
	CAdmin_thread.start();

	DEBUG_PRINT("finished");
	return 0;
}

