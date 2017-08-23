/*
 * NNMReaderFirst.cpp
 *
 *  Created on: 30 avr. 2011
 *      Author: !!M
 */
#include "NNMReaderFirst.h"
#include "Data/resources.h"

NNMReaderFirst::NNMReaderFirst(bool aFullscreen, const char* aFilename) :
GLWindow ("!!M Reader", aFullscreen),
filename (aFilename)
{
	// No GL function should be here! (OpenGL not yet initialized!)
}

NNMReaderFirst::~NNMReaderFirst()
{
	// Do not use GL function here! (OpenGL is already uninitialized)
}

void NNMReaderFirst::glInit()
{
	this->scene	= new SceneIntro(this, this->filename);
	this->scene->onEnter();
	uFMOD_PlaySong((void*)APP_SOUND, 0, XM_RESOURCE);
}

void NNMReaderFirst::glShutdown()
{
	uFMOD_StopSong();
	this->scene->onExit();
	delete(this->scene);
}

void NNMReaderFirst::renderTick(double aDelta)
{
	if(!this->scene->render(aDelta))
		this->quit();
}

void NNMReaderFirst::keyPressed(unsigned int aKey)
{
	this->scene->onKeyPressed(aKey);
}
