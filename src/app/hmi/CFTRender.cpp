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

#include "CFTRender.h"

//macros for Freetype fixpoint
#define FT_CEIL(X)	(((X + 63) & -64) >> 6)
#define FT_FLOOR(X)	((X & -64) >>6)

// static object initialization
FT_Library CFTRender::sFreeType = NULL;
FTC_Manager CFTRender::sFTCManager = NULL;
FTC_CMapCache CFTRender::sFTMapChache = NULL;
FTC_SBitCache CFTRender::sFTGlyphBitmapCache = NULL;
FT_Size CFTRender::sCurFTFont = NULL;
FTC_ScalerRec_ CFTRender::sCurFTFontAttributes;
int CFTRender::sDescender;
int CFTRender::sAscender;
int CFTRender::sInternalLeading;

// var for holding the font base path - static var, because it is used in static
// callback-function for the freetype-lib. Can't be used before initializing in
// constructor of FTRender...

const char* CFTRender::sFontBasePath;

// define font data
#if defined(__QNX__)
CFontDescriptor CFTRender::sFontDescriptor[] =
{
{ "Vera.ttf", false },
{ "VeraMoBd.ttf", false },
{ "VeraIt.ttf", false },
{ "VeraBd.ttf", false },
{ "VeraBI.ttf", false }
};
#else
CFontDescriptor CFTRender::sFontDescriptor[] =
{
{ "FreeSans.ttf", false },
{ "FreeMono.ttf", false },
{ "FreeSansOblique.ttf", false },
{ "FreeSansBold.ttf", false },
{ "FreeSansBoldOblique.ttf", false }
};
#endif


CHMIGUIImage CFTRender::sGUIImage;

CFTRender::CFTRender(const int cacheSize) :
	mSpacing(0)
{
	//init freetype
	FT_Init_FreeType(&sFreeType);

	//init the freetype cache manager
	FTC_Manager_New(sFreeType, NUM_FONTS, 10, cacheSize, FTCM_RequestCallback,
			NULL, &sFTCManager);

	//init the freetype charmap cache
	FTC_CMapCache_New(sFTCManager, &sFTMapChache);

	//init the freetype small bitmap cache (for glyph bitmaps)
	FTC_SBitCache_New(sFTCManager, &sFTGlyphBitmapCache);

	sCurFTFontAttributes.face_id = (FTC_FaceID) NORMAL;
	sCurFTFontAttributes.pixel = true; //we use pixel sizes
	sCurFTFontAttributes.height = FONT_SIZE_DEFAULT;
	sCurFTFontAttributes.width = 0;
	// get the value of the environment var ICMFW_DATA_BASE
	// if not set, we won't consider a base path
#if defined(__SH__)
	sFontBasePath = "/mediadatabase/icm-c1/font/";
#elif defined(__linux__)
	sFontBasePath = "/usr/share/fonts/truetype/freefont/";
#elif defined(__QNX__)
	sFontBasePath = "/mediadatabase/font/";
#endif

	if(sFontBasePath == NULL) {
		fprintf(stderr,"no font base path defined\n");
		exit(EXIT_FAILURE);
	}

}

CFTRender::~CFTRender(void)
{
	// cleaning up the caching
	FTC_Manager_Done(sFTCManager);
	sFTCManager = NULL;
	// cleaning up the freetype library
	FT_Done_FreeType(sFreeType);
	sFreeType = NULL;
}

void CFTRender::setFont(eFonts fontStyle)
{
	sCurFTFontAttributes.face_id = (FTC_FaceID) fontStyle;
	getFontFace();
}

void CFTRender::setFontSize(int fontSize)
{
	sCurFTFontAttributes.height = fontSize;
	getFontFace();
}

void CFTRender::setForegroundColor(float red, float green, float blue,
		float alpha)
{
	sGUIImage.setBiasColor(red, green, blue, alpha);
}

void CFTRender::getStringExtent(const char * const text, int & width,
		int & height)
{
	width = 0;
	height = 0;

	int numOfChars;

	if (NULL == text || NULL == sCurFTFont)
		return;

	if ((numOfChars = strlen(text)) == 0)
		return;

	bool useKerning = 0 != FT_HAS_KERNING(sCurFTFont->face);
	int glyphIndex = 0;
	int previousGlyphIndex = 0;
	FTC_ImageTypeRec imageType;
	FTC_SBit glyphBitmap;

	int i;
	for (i = 0; i < numOfChars; i++)
	{
		// convert character code to glyph index
		glyphIndex = FTC_CMapCache_Lookup(sFTMapChache,
				sCurFTFontAttributes.face_id, 0, text[i]);

		// retrieve kerning distance and move pen position
		if (useKerning && previousGlyphIndex && glyphIndex)
		{
			FT_Vector delta;
			FT_Get_Kerning(sCurFTFont->face, previousGlyphIndex, glyphIndex,
					FT_KERNING_DEFAULT, &delta);
			// Subpixel
			width += FT_CEIL(delta.x);
		}

		imageType.face_id = sCurFTFontAttributes.face_id;
		imageType.height = sCurFTFontAttributes.height;
		imageType.width = sCurFTFontAttributes.width;
		imageType.flags = FT_LOAD_DEFAULT; // FT_LOAD_DEFAULT?
		FTC_SBitCache_Lookup(sFTGlyphBitmapCache, &imageType, glyphIndex,
				&glyphBitmap, NULL);

		width += glyphBitmap->xadvance;
		// record current glyph index for kerning
		previousGlyphIndex = glyphIndex;
	}

	height = sAscender - sDescender - sInternalLeading;
}

void CFTRender::drawString(const char * const text, int posX, int posY)
{
	int numOfChars;
	if (NULL == text || (numOfChars = strlen(text)) == 0 || NULL == sCurFTFont)
		return;
	// Standardeinstellung
	CHMIGUIImageDesc::ePixelType pixelType = CHMIGUIImageDesc::UNSIGNED_BYTE;
	//the position of a character relative to posX and posY (upper left pixel of the glyph)
	int xOffset = 0, yOffset = 0;
	//the indices in the charmap of current and previous character (for kerning)
	int glyphIndex = 0;
	int previousGlyphIndex = 0;
	bool useKerning = 0 != FT_HAS_KERNING(sCurFTFont->face);
	FT_Vector delta; //for kerning
	FTC_SBit glyphBitmap;
	//used to get a glyph image
	FTC_ImageTypeRec imageType;
	imageType.face_id = sCurFTFontAttributes.face_id;
	imageType.height = sCurFTFontAttributes.height;
	imageType.width = sCurFTFontAttributes.width;
	imageType.flags = FT_LOAD_RENDER; // FT_LOAD_DEFAULT

	/* setting up the CGUIImage - we have to flip because the OpenGL coordinate
	 * system uses a different orientation as the freetype-library.
	 */
	sGUIImage.setFlipV(true);

#ifdef ICM_DEBUG
	printf("CFTRender::drawString \n");
#endif

	int i;
	for (i = 0; i < numOfChars; i++)
	{
		// convert character code to glyph index
		glyphIndex = FTC_CMapCache_Lookup(sFTMapChache,
				sCurFTFontAttributes.face_id, 0, text[i]);

		// retrieve kerning distance between two characters
		if (useKerning && previousGlyphIndex && glyphIndex)
		{
			FT_Get_Kerning(sCurFTFont->face, previousGlyphIndex, glyphIndex,
					FT_KERNING_DEFAULT, &delta);
			// Subpixel
			xOffset += FT_CEIL(delta.x);
		}

		//get the glyph image from cache
		FTC_SBitCache_Lookup(sFTGlyphBitmapCache, &imageType, glyphIndex,
				&glyphBitmap, NULL);
		// record current glyph index for kerning
		previousGlyphIndex = glyphIndex;

		// calculates the offset for up-right coordinate system
		yOffset = -glyphBitmap->height + glyphBitmap->top;

		// adjust the position of the current glyph
		int GlyphPosX = posX + xOffset;
		int GlyphPosY = posY + yOffset;

		/* set the imagedata into a CGUIImage, which is used as a surface and
		 * draws at calculated position
		 */
		sGUIImage.setImageData((int*) glyphBitmap->buffer,
				CHMIGUIImageDesc::ALPHA_BIAS, pixelType,
				(int) glyphBitmap->width, (int) glyphBitmap->height,
				glyphBitmap->pitch);
		sGUIImage.drawToScreen(GlyphPosX, GlyphPosY);

		//move to the position of the next character
		//TODO prio 5 :: Subpixel positioning
		xOffset += glyphBitmap->xadvance;
		// keep the current glyph index in mind
		previousGlyphIndex = glyphIndex;
	}
}

bool CFTRender::getFontFace()
{
	//get the face from the cache manager into the
	int FTError = FTC_Manager_LookupSize(sFTCManager, &sCurFTFontAttributes,
			&sCurFTFont);
	if (0 != FTError)
	{
		printf("CFTRender::getFontFace() - couldn't lookup font with id \n");
		return false;
	}

	/* updating the font information for the new font face
	 * - internal leading
	 * - descender
	 * - ascender
	 */

	int freetypeScale = sCurFTFont->metrics.y_scale;
	int EMSpace =
			FT_CEIL(FT_MulFix(sCurFTFont->face->units_per_EM, freetypeScale));
	sDescender = FT_CEIL(FT_MulFix(sCurFTFont->face->descender, freetypeScale));
	sAscender = FT_CEIL(FT_MulFix(sCurFTFont->face->ascender, freetypeScale));
	sInternalLeading = sAscender - sDescender - EMSpace;

	/* TODO prio 5 :: if we want to provide UNICODE charmapsupport, we
	 * can get the index of the unicode charmap for charmap lookups.
	 * could be saved in class
	 */

	return true;
}

FT_Error CFTRender::FTCM_RequestCallback(FTC_FaceID face_id,
		FT_Library library, FT_Pointer request_data, FT_Face *aface)
{
	// check if the ID is valid as it should be ;)
	long fontID = (long) face_id;
	if (fontID < 0 || fontID > NUM_FONTS)
	{
		aface = NULL;
		return -1;
	}

	char temp[60];
	strcpy(temp, sFontBasePath);
	strcat(temp, sFontDescriptor[fontID].mFontPath);
	// loads the FontFace with given filename
	int error = FT_New_Face(sFreeType, temp, 0, aface);

	FT_Error retVal = -1;

	if (FT_Err_Unknown_File_Format == error)
	{
		printf(
				"CFTRender::FTCM_RequestCallback() - error: unkown fontfile-format\n");
		aface = NULL;
	}
	else if (error)
	{
		printf("CFTRender::FTCM_RequestCallback() - error - filename \n");
		aface = NULL;
	}
	else
	{
		printf("CFTRender::FTCM_RequestCallback() - success: loaded font \n");
		retVal = 0;
	}

	return retVal;
}
