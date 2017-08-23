#ifndef BITMAPFONT_H
#define BITMAPFONT_H

#include "../../Engine/Core/Elements.h"
#include "../../Engine/Texture/Texture.h"
#include <string.h>
#include <GL/gl.h>

class BitmapFont
{
	public:
					BitmapFont	(int aTextureSize, int aCharWidth, int aCharHeight, float aMargeOffset = 0.0f);
		virtual		~BitmapFont	();
		void		print		(const char* aText);

	private:
		GLuint			listBase;
};

#endif // BITMAPFONT_H
