/*
 * SceneIntro.h
 *
 *  Created on: 29 mai 2011
 *      Author: !!M
 */

#ifndef SCENEINTRO_H_
#define SCENEINTRO_H_

#include "../Scene.h"
#include "../../Effects/Flash/Flash.h"
#include "../../Meshes/PlaneMesh.h"
#include "../../Meshes/Icosahedron.h"
#include "../../Meshes/RibbonMesh.h"
#include "../../Font/BitmapFont.h"
#include "../../../Utils/Utils.h"
#include "../../../Engine/Core/Elements.h"
#include "../../../Engine/Mesh/Vertex.h"
#include "../../../Engine/Texture/Texture.h"
#include "../../../Data/resources.h"
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>
#include <math.h>
#include <uFmod/ufmod.h>

class SceneIntro : public Scene
{
	public:
						SceneIntro		(GLWindow* aGLWindow, const char* aFilename);
		virtual			~SceneIntro		();
		virtual	bool	render			(double aDelta);
		virtual void	onEnter			();
		virtual	void	onExit			();
		virtual	void	onKeyPressed	(unsigned int aKey);
};

#endif /* SCENEINTRO_H_ */
