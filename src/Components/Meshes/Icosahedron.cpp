/*
 * Icosahedron.cpp
 *
 *  Created on: 10 mai 2011
 *      Author: !!M
 */

#include "Icosahedron.h"

static float vdata[18][3] =
{
	{-1.000000f, 0.000000f, 0.000000f},
	{-0.707106f, -0.707106f, 0.000000f},
	{-0.707106f, 0.000000f, -0.707106f},
	{-0.707106f, 0.000000f, 0.707106f},
	{-0.707106f, 0.707106f, 0.000000f},
	{0.000000f, -1.000000f, 0.000000f},
	{0.000000f, -0.707106f, -0.707106f},
	{0.000000f, -0.707106f, 0.707106f},
	{0.000000f, 0.000000f, -1.000000f},
	{0.000000f, 0.000000f, 1.000000f},
	{0.000000f, 0.707106f, -0.707106f},
	{0.000000f, 0.707106f, 0.707106f},
	{0.000000f, 1.000000f, 0.000000f},
	{0.707106f, -0.707106f, 0.000000f},
	{0.707106f, 0.000000f, -0.707106f},
	{0.707106f, 0.000000f, 0.707106f},
	{0.707106f, 0.707106f, 0.000000f},
	{1.000000f, 0.000000f, 0.000000f}
};

static int idata[32][3] =
{
	{0, 2, 1},
	{0, 4, 2},
	{1, 6, 5},
	{1, 3, 0},
	{1, 7, 3},
	{2, 6, 1},
	{2, 8, 6},
	{2, 10, 8},
	{3, 4, 0},
	{3, 11, 4},
	{4, 10, 2},
	{4, 12, 10},
	{5, 13, 7},
	{5, 7, 1},
	{6, 14, 13},
	{6, 13, 5},
	{7, 15, 9},
	{7, 9, 3},
	{9, 11, 3},
	{8, 14, 6},
	{10, 14, 8},//20
	{9, 15, 11},
	{11, 12, 4},
	{10, 16, 14},
	{15, 16, 11},
	{13, 15, 7},
	{12, 16, 10},
	{11, 16, 12},
	{13, 17, 15},
	{14, 17, 13},
	{16, 17, 14},
	{15, 17, 16}
};

Icosahedron::Icosahedron(T4F aColor1, T4F aColor2) :
Mesh(VT_V3FN3FC4F, GL_TRIANGLES)
{
	int i;
	std::vector<VTXV3FN3FC4F> vertices;

	for (i = 31; i >= 0; i--)
	{
		T4F color;
		if(i & 1) color = aColor1;
		else color = aColor2;

		T3F n0 = Utils::normalize((T3F&)vdata[idata[i][0]]);
		T3F n1 = Utils::normalize((T3F&)vdata[idata[i][1]]);
		T3F n2 = Utils::normalize((T3F&)vdata[idata[i][2]]);

		vertices.push_back(VTXV3FN3FC4F((T3F&)vdata[idata[i][0]], n0, color));
		vertices.push_back(VTXV3FN3FC4F((T3F&)vdata[idata[i][1]], n1, color));
		vertices.push_back(VTXV3FN3FC4F((T3F&)vdata[idata[i][2]], n2, color));
	}
	this->vertices = new ArrayBufferARB(GL_STATIC_DRAW_ARB, &vertices[0], vertices.size(), sizeof(VTXV3FN3FC4F));
}

Icosahedron::~Icosahedron()
{}

void Icosahedron::render() const
{
	glDisable			(GL_DEPTH_TEST);
	glCullFace			(GL_FRONT);
	glEnable			(GL_BLEND);
	glBlendFunc			(GL_ONE, GL_SRC_COLOR);
	Mesh::render		();

	glCullFace			(GL_BACK);
	glBlendFunc			(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Mesh::render		();
	glDisable			(GL_BLEND);
	glEnable			(GL_DEPTH_TEST);
}
