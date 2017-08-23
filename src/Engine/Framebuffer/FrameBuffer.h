/*
 * FrameBuffer.h
 *
 *  Created on: 13 mai 2011
 *      Author: !!M
 *
 *      Some parts of this code was borrowed from nvidia's demo.
 */

#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

#include "../Core/GLExtLoader.h"

class FrameBuffer
{
	public:
				FrameBuffer			();
		virtual	~FrameBuffer		();
		void	attachTexture		(GLenum aTexTarget, GLuint aTexId, GLenum aAttachment, int aMipLevel = 0, int aSlice = 0);
		void	attachRenderBuffer	(GLuint aRenderBufferId, GLenum aAttachment);
		void	unAttach			(GLenum aAttachment);
		GLuint	getId				();

	protected:
		GLuint	id;
};

#endif /* FRAMEBUFFER_H_ */
