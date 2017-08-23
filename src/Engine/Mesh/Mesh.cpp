/*
 * Mesh.cpp
 *
 *  Created on: 2 mai 2011
 *      Author: !!M
 */

#include "Mesh.h"

Mesh::Mesh(VertexType aVertexType, GLenum aPrimitiveType) :
primitiveType	(aPrimitiveType),
vertexType		(aVertexType),
vertices		(NULL),
indices			(NULL)
{}

Mesh::~Mesh()
{
	if(this->vertices)	delete(this->vertices);
	if(this->indices)	delete(this->indices);
}

void Mesh::render() const
{
	if(this->indices)
	{
		glDrawElements(this->primitiveType, this->indices->getCount(), GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(this->primitiveType, 0, this->vertices->getCount());
	}
}

void Mesh::setState()
{
	GLExtLoader::glBindBufferARB		(GL_ARRAY_BUFFER_ARB, this->vertices->getId());
	switch(this->vertexType)
	{
		case VT_V3FT2F:
			glVertexPointer		(3, GL_FLOAT, sizeof(VTXV3FT2F), (void*) 0);		// 0
			glTexCoordPointer	(2, GL_FLOAT, sizeof(VTXV3FT2F), (void*) 12);	//  + sizeof(T3F) => 12;
			glEnableClientState	(GL_VERTEX_ARRAY);
			glEnableClientState	(GL_TEXTURE_COORD_ARRAY);
		break;

		case VT_V3FN3F:
			glVertexPointer		(3, GL_FLOAT, sizeof(VTXV3FN3F), (void*) 0);	//
			glNormalPointer		(GL_FLOAT, sizeof(VTXV3FN3F), (void*) 12);		// + sizeof(T3F) => 12;
			glEnableClientState	(GL_VERTEX_ARRAY);
			glEnableClientState	(GL_NORMAL_ARRAY);
		break;

		case VT_V3FC4F:
			glVertexPointer		(3, GL_FLOAT, sizeof(VTXV3FC4F), (void*) 0);	//
			glColorPointer		(4, GL_FLOAT, sizeof(VTXV3FC4F), (void*) 12);	// + sizeof(T3F) => 12;
			glEnableClientState	(GL_VERTEX_ARRAY);
			glEnableClientState	(GL_COLOR_ARRAY);
		break;

		case VT_V3FN3FC4F:
			glVertexPointer		(3, GL_FLOAT, sizeof(VTXV3FN3FC4F), (void*) 0);	//
			glNormalPointer		(GL_FLOAT, sizeof(VTXV3FN3FC4F), (void*) 12);// + sizeof(T3F) => 12;
			glColorPointer		(4, GL_FLOAT, sizeof(VTXV3FN3FC4F), (void*) 24);// + sizeof(T3F) => 24;
			glEnableClientState	(GL_VERTEX_ARRAY);
			glEnableClientState	(GL_NORMAL_ARRAY);
			glEnableClientState	(GL_COLOR_ARRAY);
		break;

		case VT_V3FC4FT2F:
			glVertexPointer		(3, GL_FLOAT, sizeof(VTXV3FC4FT2F), (void*) 0);		// 0
			glColorPointer		(4, GL_FLOAT, sizeof(VTXV3FC4FT2F), (void*) 12);	//  + sizeof(T3F) = 12;
			glTexCoordPointer	(2, GL_FLOAT, sizeof(VTXV3FC4FT2F), (void*) 28);	//  + sizeof(T4F) => 28;
			glEnableClientState	(GL_VERTEX_ARRAY);
			glEnableClientState	(GL_COLOR_ARRAY);
			glEnableClientState	(GL_TEXTURE_COORD_ARRAY);
		break;

		case VT_V2FC4FT2F:
			glVertexPointer		(2, GL_FLOAT, sizeof(VTXV2FC4FT2F), (void*) 0);		// 0
			glColorPointer		(4, GL_FLOAT, sizeof(VTXV2FC4FT2F), (void*) 8);		//  + sizeof(T2F) = 8;
			glTexCoordPointer	(2, GL_FLOAT, sizeof(VTXV2FC4FT2F), (void*) 24);	//  + sizeof(T4F) => 24;
			glEnableClientState	(GL_VERTEX_ARRAY);
			glEnableClientState	(GL_COLOR_ARRAY);
			glEnableClientState	(GL_TEXTURE_COORD_ARRAY);
		break;
	}
}

void Mesh::unsetState()
{
	glDisableClientState			(GL_VERTEX_ARRAY);
	glDisableClientState			(GL_COLOR_ARRAY);
	glDisableClientState			(GL_NORMAL_ARRAY);
	glDisableClientState			(GL_TEXTURE_COORD_ARRAY);
	GLExtLoader::glBindBufferARB	(GL_ARRAY_BUFFER_ARB, 0);

}
