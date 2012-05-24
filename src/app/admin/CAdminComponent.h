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
#define _CADMINCOMPONENT_H

#ifndef _GLOBAL_H
   #include "Global.h"
#endif

#ifndef _CTHREAD_H
  #include "CThread.h"
#endif

#ifndef _ACOMPONENTBASE_H
   #include "AComponentBase.h"
#endif

#ifndef _CHMICOMPONENT_H
	#include "CHmiComponent.h"
#endif

#ifndef _COPENGLCOMPONENT_H
	#include "COpenGLComponent.h"
#endif

#ifndef _CTUNERCOMPONENT_H
	#include "CTunerComponent.h"
#endif

#ifndef _CNAVICOMPONENT_H
	#include "CNaviComponent.h"
#endif

#ifndef CGPSCOMPONENT_H_
	#include "CGpsComponent.h"
#endif

#ifndef CGPSFILEPARSERCOMPONENT_H_
	#include "CGpsFileParserComponent.h"
#endif

#ifndef _CCDCOMPONENT_H
	#include "CCdComponent.h"
#endif

#ifndef _CINPUTCOMPONENT_H
	#include "CInputComponent.h"
#endif

#ifndef _CMINICOMDRIVER_H
	#include "CMiniComDriver.h"
#endif

#ifndef _CKEYBOARDCAPTURETHREAD_H
	#include "CKeyboardCaptureThread.h"
#endif

#ifndef _H_CMAINDISPATCHER
	#include "CMainDispatcher.h"
#endif

#include <signal.h>
#include <unistd.h>

class CAdminComponent: public AComponentBase
{
 public:
   CAdminComponent(CComponentContext& theContext);

   virtual ~CAdminComponent();

   virtual void init(void) ;
   virtual void run(void);

   virtual void handleMessage(const CMessage& msg);

   static void terminationHandler(int sig);

 private:
   static pid_t mProcessIDs[NUM_OF_COMPONENTS];
};

#endif // _CADMINCOMPONENT_H
