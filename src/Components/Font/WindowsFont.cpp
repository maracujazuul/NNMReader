/*
 * WindowsFont.cpp
 *
 *  Created on: 18 sept. 2011
 *      Author: !!M
 */

#include "WindowsFont.h"

WindowsFont::WindowsFont(HDC aWindowHDC, const char* aFontName, int aCharSize)
{
	HFONT   font;                       // Windows Font ID
	HFONT   oldfont;                    // Used For Good House Keeping

	this->listBase = glGenLists(256);

	font = CreateFont(  aCharSize,     // Height Of Font ( NEW )
						0,              // Width Of Font
						0,              // Angle Of Escapement
						0,              // Orientation Angle
						FW_NORMAL,      // Font Weight
						FALSE,          // Italic
						FALSE,          // Underline
						FALSE,          // Strikeout
						OEM_CHARSET,   // Character Set Identifier
						OUT_TT_PRECIS,  // Output Precision
						CLIP_DEFAULT_PRECIS,        // Clipping Precision
						ANTIALIASED_QUALITY,        // Output Quality
						FF_DONTCARE|DEFAULT_PITCH,  // Family And Pitch
						aFontName);

	oldfont 			= (HFONT)SelectObject(aWindowHDC, font);       // Selects The Font We Want
	wglUseFontBitmaps	(aWindowHDC, 0, 255, this->listBase);           // Builds 96 Characters Starting At Character 32
	SelectObject		(aWindowHDC, oldfont);             // Selects The Font We Want
	DeleteObject		(font);
}

WindowsFont::~WindowsFont()
{
	glDeleteLists(this->listBase, 255);
}

void WindowsFont::print(float aX, float aY, const char* aText)
{
	glRasterPos2f	(aX, aY);
	glCallLists		(strlen(aText), GL_UNSIGNED_BYTE, aText);
}
