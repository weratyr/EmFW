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

#include "CHMIGUIImage.h"

#ifndef GRAPHICSINCLUDES_H_
#include "GraphicsIncludes.h"
#endif

/**
 * default setting for class members
 */
CHMIGUIImage::CHMIGUIImage(void) : mFlipV(false)
{
	mBiasColor.mRedComponent = 0.0;
	mBiasColor.mGreenComponent = 0.0;
	mBiasColor.mBlueComponent = 0.0;
	mBiasColor.mAlphaComponent = 0.0;
}

/**
 * \advanced constructor
 *
 * \param pixelData pixel data to be drawn
 * \param pixelFormat format of data
 * \param pixelType datatype of data
 * \param width
 * \param height
 * \param pitch currently used for drawing on GF with 1 bit per pixel format
 * 			the pitch is the width of the image in byte
 */
CHMIGUIImage::CHMIGUIImage(int* pixelData, CHMIGUIImageDesc::ePixelFormat pixelFormat, CHMIGUIImageDesc::ePixelType pixelType, int width, int height, int pitch) : mFlipV(false)
{
	mImageDesc.mPixelData = pixelData;
	mImageDesc.mPixelFormat = CHMIGUIImageDesc::ALPHA_BIAS;
	mImageDesc.mPixelType = CHMIGUIImageDesc::UNSIGNED_BYTE;
	mImageDesc.mWidth = width;
	mImageDesc.mHeight = height;
	mImageDesc.mPitch = pitch;
	mBiasColor.mRedComponent = 1.0;
	mBiasColor.mGreenComponent = 1.0;
	mBiasColor.mBlueComponent = 1.0;
	mBiasColor.mAlphaComponent = 1.0;
}

CHMIGUIImage::~CHMIGUIImage()
{
	// nothing to do - because we don't touch the image data
}

/**
 * \brief sets the pixel data to draw
 * \param pixelData pixel data to be drawn
 * \param pixelFormat format of data
 * \param pixelType datatype of data
 * \param width
 * \param height
 * \param pitch currently used for drawing on GF with 1 bit per pixel format
 * 			the pitch is the width of the image in byte
 */
void CHMIGUIImage::setImageData(int* pixelData, CHMIGUIImageDesc::ePixelFormat pixelFormat, CHMIGUIImageDesc::ePixelType pixelType, int width, int height, int pitch)
{
	mImageDesc.mPixelData = pixelData;
	mImageDesc.mPixelFormat = CHMIGUIImageDesc::ALPHA_BIAS;
	mImageDesc.mPixelType = CHMIGUIImageDesc::UNSIGNED_BYTE;
	mImageDesc.mWidth = width;
	mImageDesc.mHeight = height;

}

/**
 * this sets the ImageData via the class CHMIGUIImageDesc
 */
void CHMIGUIImage::setImageData(CHMIGUIImageDesc & imageDesc)
{
	mImageDesc.mPixelData = imageDesc.mPixelData;
	mImageDesc.mPixelFormat = CHMIGUIImageDesc::ALPHA_BIAS;
	mImageDesc.mPixelType = CHMIGUIImageDesc::UNSIGNED_BYTE;
	mImageDesc.mWidth = imageDesc.mWidth;
	mImageDesc.mHeight = imageDesc.mHeight;
}

/**
 * sets the foreground color to draw with
 * \param red
 * \param green
 * \param blue
 * \param alpha
 */
void CHMIGUIImage::setBiasColor(float red, float green, float blue, float alpha)
{
	mBiasColor.mRedComponent = red;
	mBiasColor.mGreenComponent = green;
	mBiasColor.mBlueComponent = blue;
	mBiasColor.mAlphaComponent = alpha;
}

/**
 * \brief draws image at given position
 *
 * uses the drawing method defined in CHMIGUIImage.h
 *
 * \param posX position in x direction
 * \param posY position in y direction. take notice of the different orientation of
 * 				the coordinate system (up-right or down-right)
 */
void CHMIGUIImage::drawToScreen(int posX, int posY)
{

	if ((mImageDesc.mWidth <= 0) || (mImageDesc.mHeight <= 0))
		return;

	switch(mImageDesc.mPixelFormat)
	{
		case CHMIGUIImageDesc::RGBA:
			if (CHMIGUIImageDesc::UNSIGNED_BYTE == mImageDesc.mPixelType)
			{
				#if DRAWING_MODE==GL_FRAMEBUFFER
					glPushAttrib(GL_CURRENT_BIT | GL_PIXEL_MODE_BIT | GL_ENABLE_BIT);
						glDisable(GL_TEXTURE_2D);
						glDisable(GL_LIGHTING);
						glEnable(GL_BLEND);
						glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
						glRasterPos2f(posX, posY);
						glPixelStorei(GL_UNPACK_ALIGNMENT ,1);

						// flipping has to be implemented
						glDrawPixels(mImageDesc.mWidth, mImageDesc.mHeight, GL_RGBA, GL_UNSIGNED_BYTE, (void *)mImageDesc.mPixelData);
					glPopAttrib();
				#elif DRAWING_MODE==GL_TEX
					drawRGBATexture(posX, posY);
				#elif DRAWING_MODE==GF_BITMAP_BLIT
					drawRGBATexture(posX, posY);
				#endif
			}
			break;
		case CHMIGUIImageDesc::RGB:
			break;
		case CHMIGUIImageDesc::ALPHA:
			break;
		case CHMIGUIImageDesc::RED:
			break;
		case CHMIGUIImageDesc::GREEN:
			break;
		case CHMIGUIImageDesc::BLUE:
			break;
		case CHMIGUIImageDesc::ALPHA_BIAS: //currently used
		{
			#ifdef ICM_DEBUG
				printf("CHMIGUIImage::drawToScreen : pixelFormat = Alpha-Bias \n");
			#endif
			if (CHMIGUIImageDesc::UNSIGNED_BYTE == mImageDesc.mPixelType)
			{
				#ifdef ICM_DEBUG
					printf("CHMIGUIImage::drawToScreen : pixelType = unsigned byte \n");
				#endif
				#if DRAWING_MODE==GL_FRAMEBUFFER
					glPushAttrib(GL_CURRENT_BIT | GL_PIXEL_MODE_BIT | GL_ENABLE_BIT);
						glDisable(GL_TEXTURE_2D);
						glDisable(GL_LIGHTING);
						glEnable(GL_BLEND);
						glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
						glRasterPos2f(posX, posY);
						glPixelStorei(GL_UNPACK_ALIGNMENT ,1);
						glPixelTransferf(GL_RED_BIAS, mBiasColor.mRedComponent);
						glPixelTransferf(GL_GREEN_BIAS, mBiasColor.mGreenComponent);
						glPixelTransferf(GL_BLUE_BIAS, mBiasColor.mBlueComponent);
						glPixelTransferf(GL_ALPHA_SCALE, mBiasColor.mAlphaComponent);
						// have to flip the image
						if (mFlipV)
							flipV();
						glDrawPixels(mImageDesc.mWidth, mImageDesc.mHeight, GL_ALPHA, GL_UNSIGNED_BYTE, (void *)mImageDesc.mPixelData);
						// undo flip, because the image might be used again
						if (mFlipV)
							flipV();
					glPopAttrib();
				#elif DRAWING_MODE==GL_TEX
					#ifdef ICM_DEBUG
					  printf("CHMIGUIImage::drawToScreen: DrawingMode = Texture \n");
					#endif
					glEnable(GL_TEXTURE_2D);
					GLuint textureName;
					glGenTextures(1, &textureName);
					glBindTexture(GL_TEXTURE_2D, textureName);
					glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

					// generate a texture
					// calculate size 2^n for texture - perhaps its faster to switch?
					int edge = (mImageDesc.mWidth>mImageDesc.mHeight) ? mImageDesc.mWidth : mImageDesc.mHeight;
					edge = (int)pow(2.0, (int)floor(log((double)edge)/log(2.0)+1.0));

					glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, edge, edge, 0, GL_ALPHA, GL_UNSIGNED_BYTE, NULL);

					glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mImageDesc.mWidth, mImageDesc.mHeight, GL_ALPHA, GL_UNSIGNED_BYTE,
							(GLvoid *)mImageDesc.mPixelData);

					// drawing the tex
					glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

					glColor4f(mBiasColor.mRedComponent, mBiasColor.mGreenComponent,
							mBiasColor.mBlueComponent, mBiasColor.mAlphaComponent);

					GLshort vertices[] = {posX, posY,
							posX+mImageDesc.mWidth, posY,
							posX+mImageDesc.mWidth, posY+mImageDesc.mHeight,
							posX, posY+mImageDesc.mHeight};

					GLfloat tex_vertices[8];
					if (mFlipV)
					{
						tex_vertices[0]=0.0;tex_vertices[1]=(float)mImageDesc.mHeight/(float)edge;
						tex_vertices[2]=(float)mImageDesc.mWidth/(float)edge;tex_vertices[3]=(float)mImageDesc.mHeight/(float)edge;
						tex_vertices[4]=(float)mImageDesc.mWidth/(float)edge;tex_vertices[5]=0.0;
						tex_vertices[6]=0.0;tex_vertices[7]=0.0;

					}
					else
					{
						tex_vertices[0]=0.0;tex_vertices[1]=0.0;
						tex_vertices[2]=(float)mImageDesc.mWidth/(float)edge;tex_vertices[3]=0.0;
						tex_vertices[4]=(float)mImageDesc.mWidth/(float)edge;tex_vertices[5]=(float)mImageDesc.mHeight/(float)edge;
						tex_vertices[6]=0.0;tex_vertices[7]=(float)mImageDesc.mHeight/(float)edge;

					}

					glEnableClientState(GL_VERTEX_ARRAY);
					glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					glVertexPointer(2, GL_SHORT, 0, vertices);
					glTexCoordPointer(2, GL_FLOAT, 0, tex_vertices);

					GLubyte allindices[] = {0,1,2,0,2,3};
					glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_BYTE, (GLubyte *)allindices);
					glFinish( );

					glDisableClientState( GL_TEXTURE_COORD_ARRAY );

               		// Textur loeschen und Texturmodus ausschalten
					glDeleteTextures(1, &textureName);
					glDisable(GL_TEXTURE_2D);
				#endif
			}
			else if (CHMIGUIImageDesc::BITMAP == mImageDesc.mPixelType)
			{
				#if DRAWING_MODE==GF_BITMAP_BLIT
					union {
						int col;
						struct
						{
							int b;
							int g;
							int r;
							int a;
						} components;
					};
					col = 0; // to avoid warning
					components.r = (int) round(mBiasColor.mRedComponent*255.0);
					components.g = (int) round(mBiasColor.mGreenComponent*255.0);
					components.b = (int) round(mBiasColor.mBlueComponent*255.0);
					components.a = 0;
				#endif
			}
			break;
		}
		default:
			printf("CHMIGUIImage::drawToScreen: no drawing mode \n");
			break;
	}
}

/**
 * \brief flips a image
 *
 * top to bottom
 */
void CHMIGUIImage::flipV(void)
{
	int *pixelSrc, *pixelDest, swap;
	int i, j, k;
	j = mImageDesc.mHeight / 2;

	for (i = 0; i < j; i++)
	{
		pixelSrc = (int *)mImageDesc.mPixelData + i * mImageDesc.mWidth;
		pixelDest = (int *)mImageDesc.mPixelData + (mImageDesc.mHeight - i - 1) * mImageDesc.mWidth;
		for (k = 0; k < mImageDesc.mWidth; k++)
		{
			swap = *pixelSrc; *pixelSrc = *pixelDest; *pixelDest = swap;
			pixelSrc++; pixelDest++;
		}
	}
}

void CHMIGUIImage::drawRGBATexture(int posX, int posY)
{
	glEnable(GL_TEXTURE_2D);
	GLuint textureName;
	glGenTextures(1, &textureName);
	glBindTexture(GL_TEXTURE_2D, textureName);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// generate a texture
	// calculate size 2^n for texture - perhaps its faster to switch?
	int edge = (mImageDesc.mWidth>mImageDesc.mHeight) ? mImageDesc.mWidth : mImageDesc.mHeight;
	edge = (int)pow(2.0, (int)floor(log((double)edge)/log(2.0)+1.0));

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, edge, edge, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mImageDesc.mWidth, mImageDesc.mHeight, GL_RGBA, GL_UNSIGNED_BYTE,
			(GLvoid *)mImageDesc.mPixelData);

	// drawing the tex
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glColor4f(1.0, 1.0, 1.0, 1.0);

	GLshort vertices[] = {posX, posY,
			posX+mImageDesc.mWidth, posY,
			posX+mImageDesc.mWidth, posY+mImageDesc.mHeight,
			posX, posY+mImageDesc.mHeight};

	GLfloat tex_vertices[8];
	if (mFlipV)
	{
		tex_vertices[0]=0.0;tex_vertices[1]=(float)mImageDesc.mHeight/(float)edge;
		tex_vertices[2]=(float)mImageDesc.mWidth/(float)edge;tex_vertices[3]=(float)mImageDesc.mHeight/(float)edge;
		tex_vertices[4]=(float)mImageDesc.mWidth/(float)edge;tex_vertices[5]=0.0;
		tex_vertices[6]=0.0;tex_vertices[7]=0.0;

	}
	else
	{
		tex_vertices[0]=0.0;tex_vertices[1]=0.0;
		tex_vertices[2]=(float)mImageDesc.mWidth/(float)edge;tex_vertices[3]=0.0;
		tex_vertices[4]=(float)mImageDesc.mWidth/(float)edge;tex_vertices[5]=(float)mImageDesc.mHeight/(float)edge;
		tex_vertices[6]=0.0;tex_vertices[7]=(float)mImageDesc.mHeight/(float)edge;

	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(2, GL_SHORT, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, tex_vertices);

	GLubyte allindices[] = {0,1,2,0,2,3};
	glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_BYTE, (GLubyte *)allindices);

	glDeleteTextures(1, &textureName);
}
