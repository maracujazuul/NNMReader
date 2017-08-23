/*
 * RibbonMesh.h
 *
 *  Created on: 7 juin 2011
 *      Author: !!M
 */
#ifndef RIBBONMESH_H
#define RIBBONMESH_H

#include "../../Engine/Core/GLExtLoader.h"
#include "../../Engine/Mesh/Mesh.h"
#include "../../Engine/Core/Elements.h"
#include "../../Engine/Mesh/Vertex.h"
#include <vector>
#include <math.h>

class RibbonMesh: public Mesh
{
	public:
							RibbonMesh	(int aLong, float aResolution, float aThinkness, float aAmplitude, T4F aColor);
		virtual				~RibbonMesh	();
		virtual	bool		update		(float aDelta);
	protected:
		float	thinkness;
		float	amplitude;
};

#endif // RIBBONMESH_H
