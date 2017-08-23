/*
 * Elements.h
 *
 *  Created on: 14 avr. 2011
 *      Author: !!M
 */

#ifndef ELEMENTS_H_
#define ELEMENTS_H_

class T2F
{
	public:
		float f0;
		float f1;

		T2F	() :
		f0(0.0f),
		f1(0.0f)
		{}

		T2F(float af0, float af1)
		{
			this->f0 = af0;
			this->f1 = af1;
		}
};

class T3F : public T2F
{
	public:
		float f2;

		T3F	() :
		T2F	(),
		f2(0.0f)
		{}

		T3F	(float af0, float af1, float af2) :
		T2F	(af0, af1),
		f2(af2)
		{}
};

class T4F : public T3F
{
	public:
		float f3;

		T4F	() :
		T3F	(),
		f3(0.0f)
		{}

		T4F	(float af0, float af1, float af2, float af3) :
		T3F	(af0, af1, af2),
		f3(af3)
		{}
};

class T2B
{
	public:
		unsigned char b0;
		unsigned char b1;

		T2B	() :
		b0(0),
		b1(0)
		{}

		T2B(unsigned char ab0, unsigned char ab1) :
		b0(ab0),
		b1(ab1)
		{}
};

class T3B : public T2B
{
	public:
		unsigned char b2;

		T3B	() :
		T2B	(),
		b2(0)
		{}

		T3B(unsigned char ab0, unsigned char ab1, unsigned char ab2) :
		T2B	(ab0, ab1),
		b2(ab2)
		{}
};

class T4B : public T3B
{
	public:
		unsigned char b3;

		T4B	() :
		T3B	(),
		b3(0)
		{}

		T4B	(unsigned char ab0, unsigned char ab1, unsigned char ab2, unsigned char ab3) :
		T3B	(ab0, ab1, ab2),
		b3(ab3)
		{}
};


#endif /* ELEMENTS_H_ */
