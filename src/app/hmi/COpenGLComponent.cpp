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

// +++++ screen settings +++++
int width = WINDOW_WIDTH;
int height = WINDOW_HEIGHT;

CommanderKeyCode keycode;
CommanderKeyCode currState;
int volume = 10;
int scan = 100;

// +++++ content +++++
// draw a rectangle in the middle of viewport:
GLshort border = 50;
GLshort verticeArray[] = {border, border, width-border, border, width-border, height-border, border, height-border};
GLbyte allindices[] = {0, 1, 2, 0, 2, 3};



void displayFunction(); //forward declaration


COpenGLComponent::COpenGLComponent(CComponentContext& theContext) : AComponentBase(theContext)
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

	COpenGLComponent::graphics = &nativeGraphics;
	COpenGLComponent::graphics->setDrawRoutine(COpenGLComponent::draw);
	COpenGLComponent::graphics->init();
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
	//keycode = msg.getOpcode();
	keycode = (CommanderKeyCode) msg.getOpcode();
	COpenGLComponent::graphics->run();

}

void displayFunction(void)
{

	/*
	 * to avoid polling,
	 * block on semaphore until a message for re-drawing arrives
	 */
	COpenGLComponent::getCurrentDispatcher()->dispatch(true);
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
	// draw some text below and above the pane
	char msg[80];

	DEBUG_PRINT("KEYCODE: %d", keycode);

	switch (keycode) {
	/*
	* TUNER 	GRÜN 	A
	* CD 		ROT 	B
	*/



	case MC_SOFT_A_SHORT: {
		DEBUG_PRINT("TUNER KEY  - ------ - - - -- OPENGL");
		glColor4f(0.0, 1.0, 0.0, 1.0);
		sprintf(msg, "TUNER is Working");
		DEBUG_PRINT("color is green --- TUNER is Working");

		currState = MC_SOFT_A_SHORT;
	}
		break;
	case MC_SOFT_B_SHORT: {
			DEBUG_PRINT("CD KEY  - ------ - - - -- OPENGL");
			glColor4f(1.0, 0.0, 0.0, 1.0);
			sprintf(msg, "CD is Working");
			DEBUG_PRINT("color is red --- CD is Working");

			currState = MC_SOFT_B_SHORT;
		}
		break;
	case MC_SOFT_ROTARY_RIGHT_UNPRESSED: {
		if(currState == MC_SOFT_A_SHORT)
		{
			DEBUG_PRINT("TUNER KEY  - ------ - - - -- OPENGL");
			glColor4f(0.0, 1.0, 0.0, 1.0);
			scan++;
			sprintf(msg, "TUNER SCAN: %d", scan);
			DEBUG_PRINT("color is green --- TUNER is Working");
		} else if(currState == MC_SOFT_B_SHORT)
		{
			DEBUG_PRINT("CD KEY  - ------ - - - -- OPENGL");
			glColor4f(1.0, 0.0, 0.0, 1.0);
			volume++;
			sprintf(msg, "CD VOLUME: %d", volume);
			DEBUG_PRINT("color is red --- CD is Working");
		}

		}
		break;
	case MC_SOFT_ROTARY_LEFT_UNPRESSED: {
		if(currState == MC_SOFT_A_SHORT)
		{
			DEBUG_PRINT("TUNER KEY  - ------ - - - -- OPENGL");
			glColor4f(0.0, 1.0, 0.0, 1.0);
			scan--;
			sprintf(msg, "TUNER SCAN: %d", scan);
			DEBUG_PRINT("color is green --- TUNER is Working");
		} else if(currState == MC_SOFT_B_SHORT)
		{
			DEBUG_PRINT("CD KEY  - ------ - - - -- OPENGL");
			glColor4f(1.0, 0.0, 0.0, 1.0);
			volume--;
			sprintf(msg, "CD VOLUME: %d", volume);
			DEBUG_PRINT("color is red --- CD is Working");
		}

		}
		break;

	default:
		DEBUG_PRINT("UNKNOWN KEY  - ------ - - - -- OPENGL");
		sprintf(msg, "  ");
		break;
	}


	static int counter = 0;
	counter++;

	DEBUG_PRINT("entered (counter=%i)",counter)

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLineWidth(2.0);

//	if (drawmsg == "CD") {
//		glColor4f(1.0, 0.0, 0.0, 1.0);
//		DEBUG_PRINT("color is red")
//
//	} else {
//		glColor4f(0.0, 1.0, 0.0, 1.0);
//		DEBUG_PRINT("color is green")
//	}

	glEnableClientState( GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_SHORT, 0, verticeArray);
	glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_BYTE,
			(GLubyte *) allindices);


	glColor4f(1.0, 1.0, 1.0, 1.0);
	graphics->renderText(border, 0, msg);
	//graphics->renderText(border, height - border, "Text above",CFTRender::FONT_SIZE_MINI);

	graphics->finishDraw();   // dependency to graphics driver

	glFinish();
}
