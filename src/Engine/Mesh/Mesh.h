/*
 * Mesh.h
 *
 *  Created on: 2 mai 2011
 *      Author: !!M
 */

#ifndef MESH_H_
#define MESH_H_

#include "Vertex.h"
#include "../Core/GLExtLoader.h"
#include "../Core/BufferARB.h"

class Mesh
{
	public:
						Mesh		(VertexType aVertexType, GLenum aPrimitiveType = GL_TRIANGLES);
		virtual			~Mesh		();
		virtual	void	render		() const;
		virtual	void	setState	();
		virtual void	unsetState	();

	protected:
		GLenum					primitiveType;
		VertexType				vertexType;
		ArrayBufferARB			*vertices;
		ElementArrayBufferARB	*indices;
};

#endif /* MESH_H_ */
