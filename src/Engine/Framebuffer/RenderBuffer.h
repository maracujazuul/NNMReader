/*
 * RenderBuffer.h
 *
 *  Created on: 13 mai 2011
 *      Author: !!M
 */

#ifndef RENDERBUFFER_H_
#define RENDERBUFFER_H_

#include "../Core/GLExtLoader.h"

class RenderBuffer
{
	public:
					RenderBuffer	(GLenum aInternalFormat, int aWidth, int aHeight);
		virtual		~RenderBuffer	();
		GLuint		getId			() const;

	private:
		GLuint		id;
};

#endif /* RENDERBUFFER_H_ */
