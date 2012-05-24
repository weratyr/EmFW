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

#ifndef _COPENGLCOMPONENT_H
#define _COPENGLCOMPONENT_H

#ifndef _GLOBAL_H
	#include "Global.h"
#endif

#ifndef _CCONTEXT_H
	#include "CContext.h"
#endif

#ifndef _ACOMPONENTBASE_H
	#include "AComponentBase.h"
#endif

#ifndef AGRAPHICSDRIVER_H_
	#include "AGraphicsDriver.h"
#endif

#ifndef GRAPHICSINCLUDES_H_
	#include "GraphicsIncludes.h"
#endif

#ifndef CGPSDATACONTAINERADAPTER_H_
	#include "CGpsDataContainerAdapter.h"
#endif

#ifndef CMAPVIEWER_H_
	#include "CMapViewer.h"
#endif

class COpenGLComponent : public AComponentBase
{
	public:
		COpenGLComponent(CComponentContext& theContext);


		//overwrite functions from base class
		virtual void init();
		virtual void run();

		virtual void handleMessage(const CMessage& msg);

		/* must be static so it can
		 * be called from the C-function
		 * 'displayFunction()' defined in
		 * CHmiGLThread.cpp
		 */
		static CDispatcher* getCurrentDispatcher();
		static void draw();

		static CDispatcher* mCurrentDispatcher;
		static AGraphicsDriver* graphics;
		
		static CMapViewer* mMapViewer;
};

#endif // _COPENGLCOMPONENT_H
