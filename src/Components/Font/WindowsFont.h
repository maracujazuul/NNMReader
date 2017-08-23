/*
 * WindowsFont.h
 *
 *  Created on: 18 sept. 2011
 *      Author: !!M
 */

#ifndef WINDOWSFONT_H_
#define WINDOWSFONT_H_

#include <windows.h>
#include <GL/gl.h>

class WindowsFont
{
	public:
					WindowsFont		(HDC aWindowHDC, const char* aFontName, int aCharSize);
		virtual		~WindowsFont	();
		void		print			(float aX, float aY, const char* aText);

		public:
		GLuint			listBase;
};

#endif /* WINDOWSFONT_H_ */
