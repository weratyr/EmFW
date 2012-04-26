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

#include "CGraphicsDriverEGL.h"

EGLDisplay display;
EGLSurface surface;
static EGLint attribute_list[] =
{ EGL_NATIVE_VISUAL_ID, 0, EGL_RED_SIZE, 5, EGL_GREEN_SIZE, 5, EGL_BLUE_SIZE,
		5, EGL_ALPHA_SIZE, EGL_DONT_CARE, EGL_DEPTH_SIZE, 16, EGL_NONE };

void CGraphicsDriverEGL::init()
{
	gf_3d_target_t target;
	gf_display_t gf_disp;
	gf_dev_t gf_dev;
	gf_layer_t layer;
	int i;
	EGLConfig config;
	EGLContext econtext;
	EGLint num_config;
	gf_dev_info_t info;
	gf_layer_info_t linfo;
	gf_display_info_t disp_info;
	gf_chroma_t chroma;
	int xpos = 0, ypos = 0;
	int layer_idx = -1;
	int display_idx = 0;

	/* initialize the graphics device */
	if (gf_dev_attach(&gf_dev, NULL, &info) != GF_ERR_OK)
	{
		fprintf(stderr, "gf_dev_attach() failed\n");
		exit(EXIT_FAILURE);
	}

	if (gf_display_attach(&gf_disp, gf_dev, display_idx, &disp_info)
			!= GF_ERR_OK)
	{
		fprintf(stderr, "gf_display_attach() failed\n");
		exit(EXIT_FAILURE);
	}

	layer_idx = disp_info.main_layer_index;

	/* get an EGL display connection */
	display = eglGetDisplay(gf_dev);

	if (display == EGL_NO_DISPLAY)
	{
		fprintf(stderr, "eglGetDisplay() failed\n");
		exit(EXIT_FAILURE);
	}

	if (gf_layer_attach(&layer, gf_disp, layer_idx, 0) != GF_ERR_OK)
	{
		fprintf(stderr, "gf_layer_attach() failed\n");
		exit(EXIT_FAILURE);
	}

	/* initialize the EGL display connection */
	if (eglInitialize(display, NULL, NULL) != EGL_TRUE)
	{
		fprintf(stderr, "eglInitialize: error 0x%x\n", eglGetError());
		exit(EXIT_FAILURE);
	}

	for (i = 0;; i++)
	{
		/* Walk through all possible pixel formats for this layer */
		if (gf_layer_query(layer, i, &linfo) != GF_ERR_OK)
		{
			fprintf(stderr, "Couldn't find a compatible frame "
				"buffer configuration on layer %d\n", layer_idx);
			exit(EXIT_FAILURE);
		}

		/*
		 * We want the color buffer format to match the layer format,
		 * so request the layer format through EGL_NATIVE_VISUAL_ID.
		 */
		attribute_list[1] = linfo.format;

		/* Look for a compatible EGL frame buffer configuration */
		if (eglChooseConfig(display, attribute_list, &config, 1, &num_config)
				== EGL_TRUE)
		{
			if (num_config > 0)
			{
				break;
			}
		}
	}

	/* create a 3D rendering target */
	if (gf_3d_target_create(&target, layer, NULL, 0, getWindowWidth(),
			getWindowHeight(), linfo.format) != GF_ERR_OK)
	{
		fprintf(stderr, "Unable to create rendering target\n");
		return;
	}

	memset(&chroma, 0, sizeof chroma);
	chroma.color0 = 0x0000ff; /* Blue */
	chroma.mode = GF_CHROMA_OP_NO_DRAW | GF_CHROMA_OP_SRC_MATCH;
	gf_layer_set_chroma(layer, &chroma);
	gf_layer_set_src_viewport(layer, 0, 0, getWindowWidth() - 1,
			getWindowHeight() - 1);
	gf_layer_set_dst_viewport(layer, xpos, ypos, xpos + getWindowWidth() - 1,
			ypos + getWindowHeight() - 1);
	gf_layer_set_filter(layer, GF_LAYER_FILTER_NONE);
	gf_layer_enable(layer);

	/*
	 * The layer settings haven't taken effect yet since we haven't
	 * called gf_layer_update() yet.  This is exactly what we want,
	 * since we haven't supplied a valid surface to display yet.
	 * Later, the OpenGL ES library calls will call gf_layer_update()
	 * internally, when  displaying the rendered 3D content.
	 */

	/* create an EGL rendering context */
	econtext = eglCreateContext(display, config, EGL_NO_CONTEXT, NULL);

	/* create an EGL window surface */
	surface = eglCreateWindowSurface(display, config, target, NULL);

	if (surface == EGL_NO_SURFACE)
	{
		fprintf(stderr, "Create surface failed: 0x%x\n", eglGetError());
		exit(EXIT_FAILURE);
	}

	/* connect the context to the surface */
	if (eglMakeCurrent(display, surface, surface, econtext) == EGL_FALSE)
	{
		fprintf(stderr, "Make current failed: 0x%x\n", eglGetError());
		exit(EXIT_FAILURE);
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

void CGraphicsDriverEGL::run()
{
	getDrawRoutine()();
	if (eglSwapBuffers(display, surface) == EGL_FALSE && eglGetError()
			== EGL_BAD_SURFACE)
	{
		DEBUG_PRINT("ERROR")
	}
}

void CGraphicsDriverEGL::finishDraw()
{
	eglWaitGL();
	eglWaitNative(0);
}

