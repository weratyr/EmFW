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

#ifndef CGRAPHICSDRIVERGLUT_H_
#define CGRAPHICSDRIVERGLUT_H_

#ifndef GRAPHICSINCLUDES_H_
	#include "GraphicsIncludes.h"
#endif
#ifndef AGRAPHICSDRIVER_H_
#include "AGraphicsDriver.h"
#endif

class CGraphicsDriverGLUT: public AGraphicsDriver
{
public:
	CGraphicsDriverGLUT(Int32 windowWidth, Int32 windowHeight);

	virtual void init();
	virtual void run();
	virtual void finishDraw();
};

#endif /* CGRAPHICSDRIVERGLUT_H_ */
