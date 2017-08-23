/*
 * BufferARB.h
 *
 *  Created on: 14 avr. 2011
 *      Author: !!M
 */

#ifndef BUFFERARB_H_
#define BUFFERARB_H_

#include "../Core/GLExtLoader.h"
#include "../../Utils/Exceptions.h"

template <GLenum Type>
class BufferARB
{
	public:
		template<class T>			BufferARB		(GLenum aDrawType, const T& aData, int aCount, int aElementSize);
		virtual						~BufferARB		();
							int		getCount		() const;
							int		getElementSize	() const;
							GLuint	getId			() const;
	private:
				GLuint	id;
		const	int		count;
		const	int		elementSize;
};

template<GLenum Type>
template<class T>
inline BufferARB<Type>::BufferARB(GLenum aDrawType, const T& aData, int aCount, int aElementSize) :
id			(0),
count		(aCount),
elementSize	(aElementSize)
{
	GLExtLoader::glGenBuffersARB	(1, &this->id);
	if(this->id == 0)
		throw SimpleError("glGenBuffersARB failed!");

	GLExtLoader::glBindBufferARB		(Type, this->id);
	GLExtLoader::glBufferDataARB		(Type, (GLsizeiptrARB)this->count*this->elementSize, NULL, aDrawType);
	GLExtLoader::glBufferSubDataARB		(Type, 0, (GLsizeiptrARB)this->count*this->elementSize, aData);
	GLExtLoader::glBindBufferARB		(Type, 0);
}

template<GLenum Type>
inline BufferARB<Type>::~BufferARB()
{
	GLExtLoader::glDeleteBuffersARB(1, &this->id);
}

template<GLenum Type>
inline GLuint BufferARB<Type>::getId() const
{
	return this->id;
}

template<GLenum Type>
inline int BufferARB<Type>::getElementSize() const
{
	return this->elementSize;
}

template<GLenum Type>
inline int BufferARB<Type>::getCount() const
{
	return this->count;
}

typedef BufferARB<GL_ARRAY_BUFFER_ARB>         	ArrayBufferARB;
typedef BufferARB<GL_ELEMENT_ARRAY_BUFFER_ARB>	ElementArrayBufferARB;
typedef BufferARB<GL_PIXEL_PACK_BUFFER_ARB>		PixelPackBufferARB;
typedef BufferARB<GL_PIXEL_UNPACK_BUFFER_ARB>	PixelUnpackBufferARB;

#endif /* BUFFERARB_H_ */
