/*
 * ProgramShader.cpp
 *
 *  Created on: 21 avr. 2011
 *      Author: !!M
 */
#include "ProgramShader.h"

ProgramShader::ProgramShader(const char* aVertexShaderSrc, const char* aFragmentShaderSrc) :
vertexShader	(NULL),
fragmentShader	(NULL)
{
	this->vertexShader		= new VertexShader		(aVertexShaderSrc);
	this->fragmentShader	= new FragmentShader	(aFragmentShaderSrc);
	this->handle			= GLExtLoader::glCreateProgramObjectARB();
	if(this->handle == 0)
		throw SimpleError("glCreateProgramObjectARB failed!");

	GLExtLoader::glAttachObjectARB	(this->handle, this->vertexShader->getHandle());
	GLExtLoader::glAttachObjectARB	(this->handle, this->fragmentShader->getHandle());
	GLExtLoader::glLinkProgramARB	(this->handle);}

ProgramShader::~ProgramShader()
{
	delete(this->vertexShader);
	delete(this->fragmentShader);
	GLExtLoader::glDeleteObjectARB(this->handle);
}

GLhandleARB ProgramShader::getHandle() const
{
	return this->handle;
}
