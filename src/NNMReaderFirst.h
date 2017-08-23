/*
 * NNMReaderFirst.h
 *
 *  Created on: 30 avr. 2011
 *      Author: !!M
 */

#ifndef NNMREADERFIRST_H_
#define NNMREADERFIRST_H_

#include "Engine/GLWindow.h"
#include "Components/Scenes/Scene.h"
#include "Components/Scenes/SceneIntro/SceneIntro.h"
#include <stdlib.h>
#include <uFmod/ufmod.h>

class NNMReaderFirst : public GLWindow
{
	public:
					NNMReaderFirst(bool aFullscreen, const char* aFilename);
		virtual		~NNMReaderFirst();

	protected:
		Scene				*scene;
		virtual void		renderTick		(double aDelta);
		virtual	void		keyPressed		(unsigned int aKey);
		virtual	void		glInit			();
		virtual	void		glShutdown		();
		const char*	filename;

};

#endif /* NNMREADERFIRST_H_ */
