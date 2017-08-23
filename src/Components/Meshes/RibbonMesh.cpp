/*
 * RibbonMesh.cpp
 *
 *  Created on: 7 juin 2011
 *      Author: !!M
 */
#include "RibbonMesh.h"
#define PI180		3.14f / 180

RibbonMesh::RibbonMesh(int aWidth, float aResolution, float aThinkness, float aAmplitude, T4F aColor) :
Mesh		(VT_V3FC4F, GL_TRIANGLE_STRIP),
thinkness	(aThinkness),
amplitude	(aAmplitude)
{
	std::vector<VTXV3FC4F> vertices;

	float x,tmpx;
	tmpx = aWidth/2.0f;
   	for (x = -tmpx ; x <= tmpx ; x+=aResolution)
   	{
   		vertices.push_back(VTXV3FC4F(T3F(x, (float) aThinkness, 0.0f), aColor));
   		vertices.push_back(VTXV3FC4F(T3F(x, 0.0f, 0.0f), aColor));
   	}
   	this->vertices = new ArrayBufferARB(GL_DYNAMIC_DRAW_ARB, &vertices[0], vertices.size(), sizeof(VTXV3FC4F));
}

RibbonMesh::~RibbonMesh()
{}

bool RibbonMesh::update(float aDelta)
{
	GLExtLoader::glBindBufferARB			(GL_ARRAY_BUFFER_ARB, this->vertices->getId());
	VTXV3FC4F* verticesData	= (VTXV3FC4F*) GLExtLoader::glMapBufferARB(GL_ARRAY_BUFFER_ARB, GL_WRITE_ONLY_ARB);
	for(int i=0; i!=this->vertices->getCount(); i++)
	{
		if (!(i & 1)) // if((i & 1) != 0)
		{
			verticesData[i].p.f1 = this->thinkness + (sinf((aDelta*2+i) * PI180) * cosf((aDelta*2+i) * PI180) * this->amplitude);
		}
	}
	GLExtLoader::glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
	return 1;
}
