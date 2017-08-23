/*
 * Icosahedron.h
 *
 *  Created on: 10 mai 2011
 *      Author: !!M
 */

#ifndef ICOSAHEDRON_H_
#define ICOSAHEDRON_H_

#include "../../Utils/Utils.h"
#include "../../Engine/Mesh/Mesh.h"
#include "../../Engine/Core/Elements.h"
#include "../../Engine/Mesh/Vertex.h"
#include <vector>
#include <math.h>

class Icosahedron : public Mesh
{
	public:
					Icosahedron		(T4F aColor1, T4F aColor2);
	virtual			~Icosahedron	();
	virtual	void	render			() const;
};

#endif /* ICOSAHEDRON_H_ */
