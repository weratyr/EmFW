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

#include "COpenGLComponent.h"

/**
 * Damit die OpenGL Grafik portierbar bleibt, darf ausschließlich der
 * Code der void COpenGLComponent::draw() Funktion am Ende dieser Klassen-
 * beschreibung editiert werden. Der Code in den Funktionen
 *
 * 		COpenGLComponent::init()
 * 		displayFunction(void)
 * 		COpenGLComponent::run()
 *
 * ist für die plattformabhängigen GL Funktionalitäten verantwortlich.
 * Daher haben diese Funktionen unverändert zu bleiben!
 */

CDispatcher* COpenGLComponent::mCurrentDispatcher = NULL;
AGraphicsDriver* COpenGLComponent::graphics = NULL;
CMapViewer* COpenGLComponent::mMapViewer = NULL;

// +++++ screen settings +++++
int width = WINDOW_WIDTH;
int height = WINDOW_HEIGHT;

// +++++ content +++++
// draw a rectangle in the middle of viewport:
GLshort border = 50;
GLshort verticeArray[] = {border, border, width-border, border, width-border, height-border, border, height-border};
GLbyte allindices[] = {0, 1, 2, 0, 2, 3};


void displayFunction(); //forward declaration


COpenGLComponent::COpenGLComponent(CComponentContext& theContext)
	: AComponentBase(theContext)
{
	mCurrentDispatcher=&mDispatcher;
}

void COpenGLComponent::init()
{
	DEBUG_PRINT("entered");
	#ifdef EGL
		static CGraphicsDriverEGL nativeGraphics(width,height);
	#elif GFX
		static CGraphicsDriverGFX nativeGraphics(width,height);
	#else
		static CGraphicsDriverGLUT nativeGraphics(width,height);
	#endif

	static CMapViewer MapViewer;
	COpenGLComponent::graphics = &nativeGraphics;
	COpenGLComponent::graphics->setDrawRoutine(COpenGLComponent::draw);
	COpenGLComponent::graphics->init();

	COpenGLComponent::mMapViewer = &MapViewer;

	/*
	 * init mapViewer object
	 */
	//CPosition myPosition (8647018, 49859128);
	CPosition myPosition (49.86, 8.6514);	// darmstadt      ueberschreiben der Default Position
    mMapViewer->setPosition(myPosition);
}

void COpenGLComponent::run()
{
	DEBUG_PRINT("entered");
	ASSERTION(COpenGLComponent::graphics != NULL);

	#ifdef QNX
		while (true) {
			displayFunction();
		}
	#endif
	#if defined LINUX || defined MAC
		glutDisplayFunc(displayFunction);
		glutMainLoop();
	#endif
}

void COpenGLComponent::handleMessage(const CMessage & msg)
{
	DEBUG_PRINT("%s handle message", mContext.getContextNamePtr());
	COpenGLComponent::graphics->run();
}

void displayFunction(void)
{
	while(true) //toDo remove dummy loop to avoid polling
	{
		COpenGLComponent::draw();
	}

	/*
	 * to avoid polling,
	 * block on semaphore until a message for re-drawing arrives
	 */
	//COpenGLComponent::getCurrentDispatcher()->dispatch(true);
}

CDispatcher* COpenGLComponent::getCurrentDispatcher()
{
	return mCurrentDispatcher;
}


/**
 * HINWEIS:
 *
 * Hier werden die GL Befehle geschrieben, die für das Erstellen
 * der 2D u. 3D Objekte verantwortlich sind. Dabei ist darauf zu
 * achten, dass diese Befehle der OpenGL ES 1.0 spezifikation,
 * wenn die Grafik auf allen Portierungen (QNX 641 u. 632, sowie
 * Linux) laufen soll.
 */
void COpenGLComponent::draw()
{
	if(mMapViewer!=0)
	{
		mMapViewer->Draw();
	}
	graphics->finishDraw();   // dependency to graphics driver
//	CThread::sleep(1000);
}
