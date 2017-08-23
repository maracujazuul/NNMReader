/*
 * Texture.cpp
 *
 *  Created on: 2 mai 2011
 *      Author: !!M
 */

#include "Texture.h"

Texture::~Texture()
{
	glDeleteTextures	(1, &this->id);
	//delete				(this->pbuffer);
}

GLuint Texture::getId() const
{
	return this->id;
}

int Texture::getWidth()const
{
	return this->width;
}

int Texture::getHeight() const
{
	return this->height;
}
