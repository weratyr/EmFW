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

#ifndef _CHMIGUIIMAGEDESC_H_
#define _CHMIGUIIMAGEDESC_H_

class CHMIGUIImageDesc
{
private:
	/**
	 * stores number of Bits needed for the DataType
	 */
	static const int mBitsForPixelType[];
	/**
	 * components of the PixelFormat
	 */
	static const int mPixelFormatComponents[];

public:
	// different pixel formats
	enum ePixelFormat
	{
		RGBA,
		RGB,
		ALPHA,
		RED,
		GREEN,
		BLUE,
		ALPHA_BIAS,
		LUMINANCE,
		LUMINANCE_ALPHA
	};

	// different pixel data types
	enum ePixelType
	{
		UNSIGNED_BYTE,
		BYTE,
		BITMAP,
		SHORT,
		UNSIGNED_SHORT,
		INT,
		UNSIGNED_INT,
		FLOAT
	};

	/**
	 * \brief constructor sets the PixelFormat and PixelType to default values
	 */
	CHMIGUIImageDesc(void) : mPixelFormat(RGBA), mPixelType(UNSIGNED_BYTE) {}

	/**
	 * \brief returns the number of Bytes needed per Pixel
	 * if you use the Bitmap PixelType you have to call
	 * \sa getBitsPerPixel
	 */
	int getBytesPerPixel(void)
	{
		return mBitsForPixelType[mPixelType]*mPixelFormatComponents[mPixelFormat]/8;
	}

	/**
	 * \brief returns the number of Bits needed per Pixel
	 * needed for calculating memory size for bitmaps of
	 * BITMAP-PixelType
	 */
	int getBitsPerPixel(void)
	{
		return mBitsForPixelType[mPixelType]*mPixelFormatComponents[mPixelFormat];
	}

	int mWidth;
	int mHeight;
	int mPitch;

	ePixelFormat mPixelFormat;
	ePixelType mPixelType;

	int *mPixelData;
};

#endif //_CHMIGUIIMAGEDESC_H_
