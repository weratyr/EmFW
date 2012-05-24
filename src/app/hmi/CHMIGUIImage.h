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

#ifndef CHMIGUIIMAGE_H_
#define CHMIGUIIMAGE_H_

#ifndef _CHMIGUIIMAGEDESC_H_
#include "CHMIGUIImageDesc.h"
#endif

#include <math.h>

/**
 * modes for drawing the image data to screen
 *
 * GL_FRAMEBUFFER 	via the GL-Framebuffer functions - glDrawPixels
 * GL_TEX			draw the image as full featured texture
 * GL_DRAW_TEX		with the GL ES Extensions for fast drawing textures to screen (supported only NG3-B1 with nVidia EMP)
 * GF_BITMAP_BLIT	blits a 1bit-Bitmap with the fast QNX GF functions (PixelType needs to be BITMAP (only NG3-B0)
 * GF_ALPHA_BLIT	blits to the alpha-layer (only NG3-B0) - currently not used
 */
#ifndef GL_FRAMEBUFFER
#define GL_FRAMEBUFFER 1
#endif
#define GL_TEX 2
#define GL_DRAW_TEX 3
#define GF_BITMAP_BLIT 4
#define GF_ALPHA_BLIT 5

#define DRAWING_MODE GL_TEX
/**
 * \brief representation of a surface
 *
 * the memory for the image data is not allocated or controlled by the class. this
 * is the job of the user (f.e. the allocation of the memory for font-glyphs is done
 * by the freetype library.
 *
 * if OpenGL texture drawing methods are used, the image is copied to the driver
 * but deleted afterwards. Repeated drawing of the same texture is better done
 * individually.
 */
class CHMIGUIImage
{
public:
	CHMIGUIImage(void);
	CHMIGUIImage(int* pixelData, CHMIGUIImageDesc::ePixelFormat pixelFormat, CHMIGUIImageDesc::ePixelType pixelType, int width, int height, int pitch=0);

	virtual ~CHMIGUIImage(void);
	void setImageData(int* pixelData, CHMIGUIImageDesc::ePixelFormat pixelFormat, CHMIGUIImageDesc::ePixelType pixelType, int width, int height, int pitch=0);
	void setImageData(CHMIGUIImageDesc & imageDesc);
	void setBiasColor(float red, float green, float blue, float alpha);
	void setFlipV(bool flip) { mFlipV = flip; }
	void drawToScreen(int posX, int posY);
	void flipV(void);
protected:
	CHMIGUIImageDesc mImageDesc;
	bool mFlipV;

	struct
	{
		float mRedComponent;
		float mGreenComponent;
		float mBlueComponent;
		float mAlphaComponent;
	} mBiasColor;

	void drawRGBATexture(int posX, int posY);
};

#endif // CHMIGUIIMAGE_H_
