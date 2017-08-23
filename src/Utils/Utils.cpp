/*
 * Utils.cpp
 *
 *  Created on: 22 avr. 2011
 *      Author: !!M
 */

#include "Utils.h"

Utils::Utils()
{}

Utils::~Utils()
{}

/*
std::string Utils::loadTextFromFile(const std::string& aFileName)
{
	FILE *fd;
	long len;
	long r;
	char *str;
	if (!(fd = fopen(aFileName.c_str(), "r")))
		throw LoadingError("fopen failed!", aFileName.c_str());

	fseek	(fd, 0, SEEK_END);
	len		= ftell(fd);
	fseek	(fd, 0, SEEK_SET);
	if (!(str = (char*) malloc(len * sizeof(char))))
		throw LoadingError("malloc failed!", aFileName.c_str());

	r			= fread(str, sizeof(char), len, fd);
	str[r - 1]	= '\0'; // null terminated string!!
	fclose		(fd);

	std::string	retStr(str);
	free		(str);
	return		retStr;
}

std::string Utils::loadTextFromFile(int aResourceId)
{
	return std::string("Not implemented yet ;)");
}
*/
#ifndef IIM_NO_SOIL
Texture* Utils::loadTextureFromFile(const char* aFileName, int aSoilForceChannel, GLenum aClamp, GLenum aFilter)
{
	int h;
	int w;
	int format;

	unsigned char* data = SOIL_load_image(aFileName, &h, &w, &format,  aSoilForceChannel);
	if(data == NULL)
	{
		throw LoadingError(aFileName, SOIL_last_result());
	}

	Texture *t = NULL;
	if(aSoilForceChannel == SOIL_LOAD_AUTO)
	{
		switch(format)
		{
			case SOIL_LOAD_L	: t = new Texture(data, w, h, GL_LUMINANCE, GL_LUMINANCE8, GL_UNSIGNED_BYTE, aClamp, aFilter); break;
			case SOIL_LOAD_LA	: t = new Texture(data, w, h, GL_LUMINANCE_ALPHA, GL_LUMINANCE8_ALPHA8, GL_UNSIGNED_BYTE, aClamp, aFilter); break;
			case SOIL_LOAD_RGB	: t = new Texture(data, w, h, GL_RGB, GL_RGB8, GL_UNSIGNED_BYTE, aClamp, aFilter); break;
			case SOIL_LOAD_RGBA	: t = new Texture(data, w, h, GL_RGBA, GL_RGBA8, GL_UNSIGNED_BYTE, aClamp, aFilter); break;
		}
	}
	else
	{
		switch(aSoilForceChannel)
		{
			case SOIL_LOAD_L	: t = new Texture(data, w, h, GL_LUMINANCE, GL_LUMINANCE8, GL_UNSIGNED_BYTE, aClamp, aFilter); break;
			case SOIL_LOAD_LA	: t = new Texture(data, w, h, GL_LUMINANCE_ALPHA, GL_LUMINANCE8_ALPHA8, GL_UNSIGNED_BYTE, aClamp, aFilter); break;
			case SOIL_LOAD_RGB	: t = new Texture(data, w, h, GL_RGB, GL_RGB8, GL_UNSIGNED_BYTE, aClamp, aFilter); break;
			case SOIL_LOAD_RGBA	: t = new Texture(data, w, h, GL_RGBA, GL_RGBA8, GL_UNSIGNED_BYTE, aClamp, aFilter); break;
		}
	}
	SOIL_free_image_data (data);

	return t;
}

Texture* Utils::loadTextureFromFile(int aResourceId, int aSoilForceChannel, GLenum aClamp, GLenum aFilter)
{
	int		h;
	int		w;
	int		format;
	HRSRC	res;
	LPVOID	resPtr;

	res = FindResource(GetModuleHandle(NULL), MAKEINTRESOURCE(aResourceId), IMAGE_BITMAP);
	if(res == NULL) throw SimpleError("FindResource failed!");

	resPtr = LoadResource(NULL, res);
	if(resPtr == NULL) throw SimpleError("LoadResource failed!");
	unsigned char* data = SOIL_load_image_from_memory((unsigned char*)resPtr, SizeofResource(NULL, res), &h, &w, &format,  aSoilForceChannel);
	if(data == NULL)
	{
		throw SimpleError(SOIL_last_result());
	}

	Texture *t = NULL;
	if(aSoilForceChannel == SOIL_LOAD_AUTO)
	{
		switch(format)
		{
			case SOIL_LOAD_L	: t = new Texture(data, w, h, GL_LUMINANCE, GL_LUMINANCE8, GL_UNSIGNED_BYTE, aClamp, aFilter); break;
			case SOIL_LOAD_LA	: t = new Texture(data, w, h, GL_LUMINANCE_ALPHA, GL_LUMINANCE8_ALPHA8, GL_UNSIGNED_BYTE, aClamp, aFilter); break;
			case SOIL_LOAD_RGB	: t = new Texture(data, w, h, GL_RGB, GL_RGB8, GL_UNSIGNED_BYTE, aClamp, aFilter); break;
			case SOIL_LOAD_RGBA	: t = new Texture(data, w, h, GL_RGBA, GL_RGBA8, GL_UNSIGNED_BYTE, aClamp, aFilter); break;
		}
	}
	else
	{
		switch(aSoilForceChannel)
		{
			case SOIL_LOAD_L	: t = new Texture(data, w, h, GL_LUMINANCE, GL_LUMINANCE8, GL_UNSIGNED_BYTE, aClamp, aFilter); break;
			case SOIL_LOAD_LA	: t = new Texture(data, w, h, GL_LUMINANCE_ALPHA, GL_LUMINANCE8_ALPHA8, GL_UNSIGNED_BYTE, aClamp, aFilter); break;
			case SOIL_LOAD_RGB	: t = new Texture(data, w, h, GL_RGB, GL_RGB8, GL_UNSIGNED_BYTE, aClamp, aFilter); break;
			case SOIL_LOAD_RGBA	: t = new Texture(data, w, h, GL_RGBA, GL_RGBA8, GL_UNSIGNED_BYTE, aClamp, aFilter); break;
		}
	}
	SOIL_free_image_data (data);

	return t;
}
#else
Texture* Utils::loadTextureFromFile(int aResourceId, GLenum aClamp, GLenum aFilter)
{
	BITMAP	BM;
	HBITMAP hBmp = (HBITMAP) LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(aResourceId), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

	if(hBmp == NULL) throw SimpleError("Can't load bitmap from resource");
	if(GetObject(hBmp, sizeof(BM), &BM) == 0) throw SimpleError("GetObject failed!");
	Texture *t = new Texture(BM.bmBits, BM.bmWidth, BM.bmHeight, GL_RGB8, GL_BGR, GL_UNSIGNED_BYTE, aClamp, aFilter);
	DeleteObject(hBmp);
	return t;
}
#endif

T3F Utils::normalize(const T3F& alvector)
{
	float norm = sqrtf(alvector.f0 * alvector.f0 + alvector.f1 * alvector.f1 + alvector.f2 * alvector.f2);
    if (fabs(norm) > std::numeric_limits<float>::epsilon())
    {
        float x = alvector.f0 / norm;
        float y = alvector.f1 / norm;
        float z = alvector.f2 / norm;
        return T3F(x, y, z);
    }
    return alvector;
}
