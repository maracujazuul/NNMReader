/*
 * Utils.h
 *
 *  Created on: 22 avr. 2011
 *      Author: !!M
 */

#ifndef UTILS_H_
#define UTILS_H_
#define IIM_NO_SOIL
#include "Exceptions.h"
#include "../Engine/Core/Elements.h"
#include "../Engine/Texture/Texture.h"
#include <GL/gl.h>
#ifndef IIM_NO_SOIL
	#include <SOIL/SOIL.h>
#endif
#include <math.h>
#include <limits>

class Utils
{
	public:
		virtual				~Utils();
		//static	const char*	loadTextFromFile		(const char* aFileName);
		//static	const char*	loadTextFromFile		(int aResourceId);
#ifndef IIM_NO_SOIL
		static	Texture*	loadTextureFromFile		(const char* aFileName, int aSoilForceChannel = SOIL_LOAD_AUTO, GLenum aClamp = GL_CLAMP, GLenum aFilter = !GL_NEAREST);
		static	Texture*	loadTextureFromFile		(int aResourceId, int aSoilForceChannel = SOIL_LOAD_AUTO, GLenum aClamp = GL_CLAMP, GLenum aFilter = !GL_NEAREST);
#else
		static	Texture*	loadTextureFromFile		(int aResourceId, GLenum aClamp = GL_CLAMP, GLenum aFilter = !GL_NEAREST);
#endif
		static	T3F			normalize				(const T3F& alvector);
	private:
		Utils();
};

#endif /* UTILS_H_ */
