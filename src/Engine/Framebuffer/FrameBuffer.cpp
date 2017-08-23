/*
 * FrameBuffer.cpp
 *
 *  Created on: 13 mai 2011
 *      Author: !!M
 */

#include "FrameBuffer.h"

FrameBuffer::FrameBuffer()
{
	GLExtLoader::glGenFramebuffersEXT(1, &this->id);
}

FrameBuffer::~FrameBuffer()
{
	GLExtLoader::glDeleteFramebuffersEXT(1, &this->id);
}

void FrameBuffer::attachTexture(GLenum aTexTarget, GLuint aTexId, GLenum aAttachment, int aMipLevel, int aSlice)
{
	if (aTexTarget == GL_TEXTURE_1D)
	{
		GLExtLoader::glFramebufferTexture1DEXT(GL_FRAMEBUFFER_EXT, aAttachment, GL_TEXTURE_1D, aTexId, aMipLevel);
	}
	else if (aTexTarget == GL_TEXTURE_3D)
	{
		GLExtLoader::glFramebufferTexture3DEXT(GL_FRAMEBUFFER_EXT, aAttachment, GL_TEXTURE_3D, aTexId, aMipLevel, aSlice);
	}
	else
	{
		GLExtLoader::glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, aAttachment, aTexTarget, aTexId, aMipLevel);
	  }
}

void FrameBuffer::attachRenderBuffer(GLuint aRenderBufferId, GLenum aAttachment)
{
	GLExtLoader::glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, aAttachment, GL_RENDERBUFFER_EXT, aRenderBufferId);
}

void FrameBuffer::unAttach(GLenum aAtachment)
{
	GLint type = 0;
	GLExtLoader::glGetFramebufferAttachmentParameterivEXT(GL_FRAMEBUFFER_EXT, aAtachment, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_EXT, &type);

	switch(type)
	{
		case GL_RENDERBUFFER_EXT:
			this->attachRenderBuffer(0, aAtachment);
			break;

		case GL_TEXTURE:
			this->attachTexture(GL_TEXTURE_2D, 0, aAtachment);
			break;
	  }
}

GLuint FrameBuffer::getId()
{
	return this->id;
}
