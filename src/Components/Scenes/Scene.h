/*
 * Scene.h
 *
 *  Created on: 26 avr. 2011
 *      Author: !!M
 */

#ifndef SCENE_H_
#define SCENE_H_

#include "../../Engine/GLWindow.h"

class Scene
{
	public:
						Scene			(GLWindow* aGLWindow);
		virtual			~Scene			();
		virtual	bool	render			(double aDelta) = 0;
		virtual	void	onEnter			() = 0;
		virtual	void	onExit			() = 0;
		virtual	void	onKeyPressed	(unsigned int aKey) = 0;

	protected:
		GLWindow		*window;
};

#endif /* SCENE_H_ */
