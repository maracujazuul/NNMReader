/*
 * Shader.h
 *
 *  Created on: 21 avr. 2011
 *      Author: !!M
 */

#ifndef SHADER_H_
#define SHADER_H_

#include "../Core/GLExtLoader.h"
#include "../../Utils/Exceptions.h"
#include <vector>

template<int Type>
class Shader
{
	public:
							Shader		(const char* aSourceCode);
		virtual				~Shader		();
				GLhandleARB	getHandle	() const;

	protected:
		GLhandleARB	handle;
};


template<int Type>
inline Shader<Type>::Shader(const char* aSourceCode) :
handle	(0)
{
	this->handle = GLExtLoader::glCreateShaderObjectARB(Type);
	if(this->handle == 0)
		throw SimpleError("glCreateShaderObjectARB failed!");

	const char *src = aSourceCode;
	GLExtLoader::glShaderSourceARB(this->handle, 1, (const GLcharARB**)&src, NULL);
	GLExtLoader::glCompileShaderARB(this->handle);

	int result;
	GLExtLoader::glGetObjectParameterivARB(this->handle, GL_OBJECT_COMPILE_STATUS_ARB, &result);
	if (!result)
	{
		//We failed to compile. So we attempt to get the length of our error log.
		int length;
		GLExtLoader::glGetObjectParameterivARB(this->handle, GL_OBJECT_INFO_LOG_LENGTH_ARB, &length);

		std::vector<char> buffer;
		buffer.resize(length);
		GLExtLoader::glGetInfoLogARB(this->handle, length, NULL, (GLcharARB*)&buffer[0]);
		throw SimpleError(&buffer[0]);
	}
}

template<int Type>
inline Shader<Type>::~Shader()
{
	GLExtLoader::glDeleteObjectARB(this->handle);
}

template<int Type>
inline GLhandleARB Shader<Type>::getHandle() const
{
	return this->handle;
}

typedef Shader<GL_VERTEX_SHADER_ARB>	VertexShader;
typedef Shader<GL_FRAGMENT_SHADER_ARB>	FragmentShader;

#endif /* SHADER_H_ */
