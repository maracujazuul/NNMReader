/*
 * PlaneMesh.cpp
 *
 *  Created on: 3 mai 2011
 *      Author: !!M
 */

#include "PlaneMesh.h"

PlaneMesh::PlaneMesh(float aWidth, float aHeight, float aTexScale) :
Mesh(VT_V3FT2F, GL_QUADS)
{
	std::vector<VTXV3FT2F> vertices;
	vertices.push_back(VTXV3FT2F(T3F(-(aWidth*0.5f),	-(aHeight*0.5f),	0.0f),	T2F(0.0f,		0.0f)));
	vertices.push_back(VTXV3FT2F(T3F(aWidth*0.5f,		-(aHeight*0.5f),	0.0f),	T2F(aTexScale,	0.0f)));
	vertices.push_back(VTXV3FT2F(T3F(aWidth*0.5f,		aHeight*0.5f,		0.0f),	T2F(aTexScale,	aTexScale)));
	vertices.push_back(VTXV3FT2F(T3F(-(aWidth*0.5f),	aHeight*0.5f,		0.0f),	T2F(0.0f,		aTexScale)));
	this->vertices = new ArrayBufferARB(GL_STATIC_DRAW_ARB, &vertices[0], vertices.size(), sizeof(VTXV3FT2F));
}

PlaneMesh::~PlaneMesh()
{}
