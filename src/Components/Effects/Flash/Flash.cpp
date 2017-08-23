/*
 * Flash.cpp
 *
 *  Created on: 11 juin 2011
 *      Author: !!M
 */

#include "Flash.h"

Flash::Flash(unsigned char aStep) :
step		(1.0f / aStep),
opacity		(0.0f),
flashState	(true),
alive		(false)
{}

Flash::~Flash()
{}

void Flash::spawn()
{
	this->alive			= true;
	this->opacity		= 0.0f;
	this->flashState	= true;
}

void Flash::render()
{
	if(flashState == true)
	{
		this->opacity += this->step;
		if(this->opacity >= 1.0f)
		{
			this->opacity		= 1.0f;
			this->flashState	= false;
		}
	}

	if(flashState == false)
	{
		this->opacity -= this->step;
		if(this->opacity <= 0.0f)
		{
			this->alive			= false;
		}
	}

	glMatrixMode			(GL_PROJECTION);
	glLoadIdentity			();
	glOrtho					(0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	glMatrixMode			(GL_MODELVIEW);
	glLoadIdentity			();
	glEnable				(GL_BLEND);
	glColor4f				(1.0f, 1.0f, 1.0f, this->opacity);
	glBegin(GL_QUADS);
		glVertex2i(0, 0);
		glVertex2i(1, 0);
		glVertex2i(1, 1);
		glVertex2i(0, 1);
	glEnd();
	glColor4f				(1.0f, 1.0f, 1.0f, 1.0f);
	glDisable				(GL_BLEND);
}

bool Flash::getState()
{
	return this->flashState;
}

bool Flash::isAlive()
{
	return this->alive;
}
