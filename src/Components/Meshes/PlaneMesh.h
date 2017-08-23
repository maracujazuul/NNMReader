/*
 * PlaneMesh.h
 *
 *  Created on: 3 mai 2011
 *      Author: !!M
 */

#ifndef PLANEMESH_H_
#define PLANEMESH_H_

#include "../../Engine/Mesh/Mesh.h"
#include "../../Engine/Core/Elements.h"
#include "../../Engine/Mesh/Vertex.h"
#include <vector>

class PlaneMesh : public Mesh
{
	public:
						PlaneMesh	(float aWidth, float aHeight, float aTexScale);
		virtual			~PlaneMesh	();
};

#endif /* PLANEMESH_H_ */
