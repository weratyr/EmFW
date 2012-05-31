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

#ifndef GRAPHICSINCLUDES_H_
#define GRAPHICSINCLUDES_H_

#ifdef __QNX__
#ifdef EGL
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <GLES/egl.h>
#include <gf/gf3d.h>
		#ifndef CGRAPHICSDRIVEREGL_H_
#include "CGraphicsDriverEGL.h"
		#endif
#elif GFX
		#ifndef GFX_H_INCLUDED
#include "gfx.h"
		#endif
#include <GLES/glext.h>
		#ifndef CGRAPHICSDRIVERGFX_H_
#include "CGraphicsDriverGFX.h"
#endif
	#endif
#elif __linux__
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glx.h>
	#ifndef CGRAPHICSDRIVERGLUT_H_
#include "CGraphicsDriverGLUT.h"
	#endif
#elif __APPLE__
#include <GLUT/glut.h>
	#ifndef CGRAPHICSDRIVERGLUT_H_
#include "CGraphicsDriverGLUT.h"
#endif
#endif


#endif /* GRAPHICSINCLUDES_H_ */
