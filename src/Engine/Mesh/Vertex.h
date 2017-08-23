/*
 * Vertex.h
 *
 *  Created on: 30 avr. 2011
 *      Author: !!M
 */

#ifndef VERTEX_H_
#define VERTEX_H_

#include "../Core/Elements.h"

enum VertexType
{
	VT_V3FT2F,
	VT_V3FN3F,
	VT_V3FN3FC4F,
	VT_V2FC4FT2F,
	VT_V3FC4F,
	VT_V3FC4FT2F
};

class VTXV3FT2F
{
	public:
		T3F p;
		T2F t;

	VTXV3FT2F()
	{}

	VTXV3FT2F(T3F aP, T2F aT) :
	p(aP),
	t(aT)
	{}

};
class VTXV3FN3F
{
	public:
		T3F	p;
		T3F n;

		VTXV3FN3F()
		{}

		VTXV3FN3F(T3F aP, T3F aN) :
		p(aP),
		n(aN)
		{}
};

class VTXV3FC4F
{
	public:
		T3F p;
		T4F c;

		VTXV3FC4F()
		{}

		VTXV3FC4F(T3F aP, T4F aC) :
		p(aP),
		c(aC)
		{}
};

class VTXV3FN3FC4F
{
	public:
		T3F p;
		T3F n;
		T4F c;

		VTXV3FN3FC4F()
		{}

		VTXV3FN3FC4F(T3F aP, T3F aN, T4F aC) :
		p(aP),
		n(aN),
		c(aC)
		{}
};

class VTXV2FC4FT2F
{
	public:
		T2F	p;
		T4F	c;
		T2F	t;

		VTXV2FC4FT2F()
		{}

		VTXV2FC4FT2F(T2F aP, T4F aC, T2F aT) :
		p(aP),
		c(aC),
		t(aT)
		{}
};

class VTXV3FC4FT2F
{
	public:
		T3F	p;
		T4F	c;
		T2F	t;

		VTXV3FC4FT2F()
		{}

		VTXV3FC4FT2F(T3F aP, T4F aC, T2F aT) :
		p(aP),
		c(aC),
		t(aT)
		{}
};

#endif /* VERTEX_H_ */
