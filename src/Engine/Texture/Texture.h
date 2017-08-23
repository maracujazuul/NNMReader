/*
 * Texture.h
 *
 *  Created on: 2 mai 2011
 *      Author: !!M
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "../../Utils/Exceptions.h"
#include "../Core/BufferARB.h"
#include "../Core/Elements.h"
#include <windows.h>
#include <GL/gl.h>
//#include <GL/glext.h>
#include <GL/glu.h>

class Texture
{
	public:
		template<class T>	Texture		(const T& aData, int aWidth, int aHeight, GLenum aInternalFormat, GLenum aTextureFormat, GLenum aPixelType, GLenum aClamp = GL_CLAMP, GLenum aFilter = !GL_NEAREST);
		virtual				~Texture	();
		GLuint				getId		() const;
		int					getWidth	() const;
		int					getHeight	() const;

	protected:
		PixelUnpackBufferARB	*pbuffer;
		GLuint					id;
		const int				width;
		const int				height;
};

template<class T>
Texture::Texture(const T& aData,  int aWidth, int aHeight, GLenum aInternalFormat, GLenum aTextureFormat, GLenum aPixelType, GLenum aClamp, GLenum aFilter) :
pbuffer		(NULL),
id			(0),
width		(aWidth),
height		(aHeight)
{
	glGenTextures(1, &this->id);
	if(this->id == 0)
		throw SimpleError("glGenTextures failed!");

	//int elementSize = 0;
	// TODO: Use more texture format!
	/*
	switch(aTextureFormat)
	{
		case GL_RGB8				: elementSize = sizeof(T3B);			break;
		case GL_RGBA8				: elementSize = sizeof(T4B);			break;
		case GL_LUMINANCE8			: elementSize = sizeof(unsigned char);	break;
		case GL_LUMINANCE8_ALPHA8	: elementSize = sizeof(T2B);			break;
		default						: throw SimpleError("Texture failed! Format not supported!");

	}
	this->pbuffer = new PixelUnpackBufferARB(GL_STATIC_DRAW_ARB, aData, aWidth*aHeight, elementSize);
	 */
	GLenum minFilter = GL_LINEAR_MIPMAP_LINEAR;
	GLenum magFilter = GL_LINEAR;
	if(aFilter == GL_NEAREST)
	{
		minFilter = GL_NEAREST;
		magFilter = GL_NEAREST;
	}
	glBindTexture					(GL_TEXTURE_2D, this->id);
	glTexParameteri					(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri					(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri					(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, aClamp);
	glTexParameteri					(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, aClamp);
	//glTexParameterf					(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.0f);
	//glBindBufferARB					(GL_PIXEL_UNPACK_BUFFER_ARB, this->pbuffer->getId());
	//glTexImage2D					(GL_TEXTURE_2D, 0, aTextureFormat, aWidth, aHeight, 0, aInternalFormat, aPixelType, 0);
	//glGenerateMipmapEXT				(GL_TEXTURE_2D);
	//glBindBufferARB					(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
	gluBuild2DMipmaps				(GL_TEXTURE_2D, aInternalFormat, aWidth, aHeight, aTextureFormat, GL_UNSIGNED_BYTE, aData);
	glBindTexture					(GL_TEXTURE_2D, 0);
}

#endif /* TEXTURE_H_ */
