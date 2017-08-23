/*
 * Exceptions.h
 *
 *  Created on: 9 avr. 2011
 *      Author: !!M
 *
 *      Base class to handle error and throw them
 *      throught the exception system
 */
#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <exception>
#include <string.h>
#include <stdlib.h>

class Exception : public std::exception
{
	public:
							Exception	(const char* aWhat = "");
		virtual				~Exception	() throw();
		virtual	const char*	what		() const throw();

	protected:
		const char*	msg;
};

class SimpleError : public Exception
{
	public:
		SimpleError(const char* aMessage);
};

class LoadingError : public Exception
{
	public:
		LoadingError(const char* aFile, const char* aReason);
};

#endif /* EXCEPTIONS_H_ */
