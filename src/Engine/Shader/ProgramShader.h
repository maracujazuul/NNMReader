/*
 * ProgramShader.h
 *
 *  Created on: 21 avr. 2011
 *      Author: !!M
 */

#ifndef PROGRAMSHADER_H_
#define PROGRAMSHADER_H_

#include "../Core/GLExtLoader.h"
#include "../../Utils/Exceptions.h"
#include "Shader.h"

class ProgramShader
{
	public:
							ProgramShader			(const char* aVertexShaderSrc, const char* aFragmentShaderSrc);
		virtual				~ProgramShader			();
		GLhandleARB			getHandle				() const;

	protected:
		VertexShader	*vertexShader;
		FragmentShader	*fragmentShader;
		GLhandleARB		handle;
};

#endif /* PROGRAMSHADER_H_ */
