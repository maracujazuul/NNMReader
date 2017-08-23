/*
 * RenderBuffer.cpp
 *
 *  Created on: 13 mai 2011
 *      Author: !!M
 */

#include "RenderBuffer.h"

RenderBuffer::RenderBuffer(GLenum aInternalFormat, int aWidth, int aHeight)
{
	GLExtLoader::glGenRenderbuffersEXT		(1, &this->id);
	GLExtLoader::glBindRenderbufferEXT		(GL_RENDERBUFFER_EXT, this->id);
	GLExtLoader::glRenderbufferStorageEXT	(GL_RENDERBUFFER_EXT, aInternalFormat, aWidth, aHeight);
	GLExtLoader::glBindRenderbufferEXT		(GL_RENDERBUFFER_EXT, 0);
}

RenderBuffer::~RenderBuffer()
{
	GLExtLoader::glDeleteRenderbuffersEXT(1, &this->id);
}

GLuint RenderBuffer::getId() const
{
	return this->id;
}
