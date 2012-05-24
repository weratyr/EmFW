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

#include "CGraphicsDriverGLUT.h"

CGraphicsDriverGLUT::CGraphicsDriverGLUT(Int32 windowWidth, Int32 windowHeight) :
	AGraphicsDriver(windowWidth, windowHeight)
{
}

void CGraphicsDriverGLUT::init()
{
	DEBUG_PRINT("entered")
	int dummy = 0;
	glutInit(&dummy, NULL);
	
	/*
	 * origin framework
	 * */
	
/*	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(getWindowWidth(), getWindowHeight());
	glutCreateWindow("OpenGLdemo");

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

	glOrtho(0, getWindowWidth(), 0, getWindowHeight(), 0, 100);*/

	/*
	 * import from MapViewer source (COpenGL::init())
	 */
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	glutInitWindowSize(getWindowWidth(), getWindowHeight());
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	
	glutCreateWindow( "OSM" );
   
	//glClearColor(1.0, 1.0, 1.0, 0.0);
	glClearColor(0.45, 0.43, 0.41, 0.0);
	//glShadeModel(GL_SMOOTH);
//	glEnable( GL_VERTEX_ARRAY );		// to do wofuer
	//glEnable( GL_BLEND );
	//glEnable(GL_LINE_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glViewport(0, 0, getWindowWidth(), getWindowHeight());
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	   
   	glOrtho( -getWindowWidth()/2.0f, getWindowWidth()/2.0f, -getWindowHeight()/2.0f, getWindowHeight()/2.0f, -100.0f, 100.0f );      // Origin = center
	  	    
	glTranslatef(0,0,-50);
	glMatrixMode(GL_MODELVIEW);
}

void CGraphicsDriverGLUT::run()
{
	getDrawRoutine()();
}

void CGraphicsDriverGLUT::finishDraw()
{
	glutSwapBuffers();
	glutPostRedisplay();
}

