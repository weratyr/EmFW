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

#ifndef _CFTRENDER_H_
#define _CFTRENDER_H_

/**
 * Font Renderer based on FreeType2 library
 * developed by Simon Kretschmer
 */

#include "ft2build.h"
#include "freetype/freetype.h"
#include "freetype/ftcache.h"

#ifndef CHMIGUIIMAGE_H_
#include "CHMIGUIImage.h"
#endif

/**
 * identifies a used font
 * static declared in a fixed-sized array - because we only use a preassigned
 * set of fonts
 * requested by user via the enum eFonts
 */
struct CFontDescriptor
{
	const char* mFontPath;
	bool mLoaded;
};

/**
 * class for rendering Truetype-Fonts using the FT2-Library
 *
 * For the rendering we use the caching-subsystem provided by the freetype
 * library. A user can draw a string (at the moment) in 6 different fonts
 * (enum eFonts). The mapping is done internally with the struct CFontDescriptor.
 *
 * drawing is delivered to the CHMIGUIImage-class, which defines an surface and
 * the possibility to draw onto screen. (\sa CHMIGUIImage). The freetype2 library
 * is able to render as an one byte-per-pixel image (antialiased), one bit-per-pixel
 * (black/white) or as an RGB-image (Subpixel-Hinting).
 * We use 1 byte-per-pixel with OpenGL and 1 bit-per-pixel with the GF-Lib on NG3-B0
 */
class CFTRender
{
public:
	enum eFonts
	{
		NORMAL,
		NORMAL_NPROP,
		ITALIC,
		BOLD,
		BOLD_ITALIC,
		NUM_FONTS
	};

	enum
	{
		FONT_SIZE_MINI = 12,
		FONT_SIZE_DEFAULT = 20,
		FONT_SIZE_MAXIMUM = 40
	};

	CFTRender(const int cacheSize);
	virtual ~CFTRender(void);

	/** Copy-constructor.
	 *  \param source Source object to be copied.
	 */
	CFTRender(const CFTRender& source) {}

	/** Operator= method.
	 * \param kSource Source object to be copied.
	 * \return This instance.
	 */
	CFTRender& operator=(const CFTRender& source) {return *this;}

	/** sets an fontstyle
	 *
	 * currently available is NORMAL, NORMAL_NPROP, ITALIC, BOLD, BOLD_ITALIC
	 * the mapping is defined internally (top of CHMIGUIImage.cpp)
	 *
	 * \param fontStyle style of the font
	 */
	void setFont(eFonts fontStyle);

	/** sets the fontsize
	 *
	 * see predefined sizes, e.g.
	 * - FONT_SIZE_DEFAULT (20)
	 * please use this defines (if possible) for a consistent apperance of
	 * the user interface
	 *
	 * \param fontSize
	 */
	void setFontSize(int fontSize);

	/**
	 * \brief sets the foreground color as four components
	 * the coloring of the glyhps is done with the help of \sa CHMIGUIImage
	 * \param red red component
	 * \param green green component
	 * \param blue blue component
	 * \param alpha alpha value - to draw nontransparent set this value to 1.0
	 */
	void setForegroundColor(float red, float green, float blue, float alpha);

	/**
	 * \brief calculates the required space in x- and y-direction of a given font.
	 *
	 * \param pkText The text.
	 * \param vWidth Reference to width of the text in pixels.
	 * \param vHeight Reference to height of the text in pixels.
	*/
	void getStringExtent(const char * const text, int & width, int & height);

	/**
	 * \brief draws the string at given position
	 *
	 * \param text text to draw
	 * \param posX position in x direction
	 * \param posY position in y direction
	 */
	void drawString(const char * const text, int posX, int posY);

protected:

	/**
	 * \brief used to get a fontface with the freetype cache of fontfaces
	 *
	 * previously set in sCurFTFontAttributes
	 */
	bool getFontFace();

    /**
	 * \brief callback function for the freetype cache manager to request a font
	 *
	 * this function calls LoadRequestedFont if necessary.
	 */
	static FT_Error FTCM_RequestCallback( FTC_FaceID face_id, FT_Library library,
											FT_Pointer request_data, FT_Face *aface );

	//a freetype instance
	static FT_Library sFreeType;
	//a freetype cache manager
	static FTC_Manager  sFTCManager;
	//a freetype charmap chache
	static FTC_CMapCache  sFTMapChache;
	//a freetype cache for the glyph images
	static FTC_SBitCache  sFTGlyphBitmapCache;
	//this object is used to lookup faces with the current fontid and
	//the current scale in freetype cache manager
	static FTC_ScalerRec_ sCurFTFontAttributes;
	//this object contains the face of the current font.
	static FT_Size sCurFTFont;

	//TODO prio 5 :: to store the index of the unicode charmap for the font with the current fontid.
	// static Uint mIndexOfUnicodeCharmap;

	//to store the metrics for the font with the current fontid.
	static int sDescender;
	static int sAscender;
	static int sInternalLeading;
	int mSpacing;

	// path to the fontfiles, needs to be static, because it's called from
	// the static callback-function...
	static const char * sFontBasePath;
	static CFontDescriptor sFontDescriptor[NUM_FONTS];

	// an static instance of the CHMIGUIImage class
		static CHMIGUIImage sGUIImage;

};

#endif //_CFTRENDER_H_
