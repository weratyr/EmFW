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

#include "CGraphicsDriverGFX.h"

GfxEglState CGraphicsDriverGFX::mGfxEglState;

void CGraphicsDriverGFX::init()
{
	memset(&mGfxEglState, 0, sizeof(mGfxEglState)); // passiert im Konstrukturaufruf
	if (!GfxEglInitialize(&mGfxEglState))
		DEBUG_PRINT("Graphics initialization failed!")
	else
	{
		DEBUG_PRINT("Graphics initialized...")
	}

	glEnable(GL_LINE_SMOOTH);
	glShadeModel(GL_SMOOTH);
	glDisable(GL_ALPHA_TEST);
	glAlphaFunc(GL_EQUAL, 1.0f);
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glViewport(0, 0, getWindowWidth(), getWindowHeight());
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glOrthof(0, getWindowWidth(), 0, getWindowHeight(), 0, 100);
}

void CGraphicsDriverGFX::run()
{
	getDrawRoutine()();
	GfxEglSwapBuffers(&mGfxEglState);
}

void CGraphicsDriverGFX::finishDraw()
{
	eglWaitGL();
	eglWaitNative(0);
}

