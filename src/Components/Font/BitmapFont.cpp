#include "BitmapFont.h"

BitmapFont::BitmapFont(int aTextureSize, int aCharWidth, int aCharHeight, float aMargeOffset)
{
	this->listBase	= glGenLists(255);

	float cellHeight	= (float) aCharHeight / aTextureSize;
	float cellWidth		= (float) aCharWidth / aTextureSize;

	unsigned char index = 0;
	for(int y=0; y<(aTextureSize/aCharHeight); y++)
	{
		float charOffsetY = y * cellHeight;
		for (int x=0; x<(aTextureSize/aCharWidth); x++)
		{
			if(index == 0x20)// " "
			{
				glNewList(this->listBase + index, GL_COMPILE);
					glTranslatef(aCharWidth-aMargeOffset, 0, 0);
				glEndList();
				index++;
				continue;
			}
			float charOffsetX = x * cellWidth;
			glNewList(this->listBase + index, GL_COMPILE);
			glBegin(GL_QUADS);
				glTexCoord2f	(charOffsetX, charOffsetY);
				glVertex2i		(0, 0);

				glTexCoord2f	(charOffsetX+cellWidth, charOffsetY);
				glVertex2i		(aCharWidth, 0);

				glTexCoord2f	(charOffsetX+cellWidth, charOffsetY+cellHeight);
				glVertex2i		(aCharWidth, aCharHeight);

				glTexCoord2f	(charOffsetX, charOffsetY+cellHeight);
				glVertex2i		(0, aCharHeight);
			glEnd();
			glTranslatef(aCharWidth-aMargeOffset, 0, 0);
			glEndList();
			index++;
		}
	}
}

BitmapFont::~BitmapFont()
{
	glDeleteLists(this->listBase, 255);
}

void BitmapFont::print(const char* aText)
{
	glListBase		(this->listBase);
	glCallLists		(strlen(aText), GL_UNSIGNED_BYTE, aText);
}
