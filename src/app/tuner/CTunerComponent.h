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

#ifndef _CTUNERCOMPONENT_H
#define _CTUNERCOMPONENT_H

#ifndef _ACOMPONENTBASE_H
   #include "AComponentBase.h"
#endif
#ifndef _CMESSAGE_H
	#include "CMessage.h"
#endif
#ifndef H_EVENTS
	#include "Events.h"
#endif
#ifndef _CCONTEXT_H
	#include "CContext.h"
#endif

class CTunerComponent: public AComponentBase
{
 public:
   CTunerComponent(CComponentContext& theContext);

   virtual ~CTunerComponent();

   virtual void init(void) ;
   virtual void run(void);

   virtual void handleMessage(const CMessage& msg);

};

#endif // _CTUNERCOMPONENT_H
