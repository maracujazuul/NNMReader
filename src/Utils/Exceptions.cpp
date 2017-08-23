/*
 * Exceptions.cpp
 *
 *  Created on: 9 avr. 2011
 *      Author: !!M
 *
 *      Base class to handle error and throw them
 *      throught the exception system
 */
#include "Exceptions.h"

Exception::Exception(const char* aMessage) : std::exception(),
msg	(aMessage)
{}

Exception::~Exception() throw()
{}

const char* Exception::what() const throw()
{
    return this->msg;
}

SimpleError::SimpleError(const char* aMessage)
{
	this->msg = aMessage;
}

LoadingError::LoadingError(const char* aFile, const char* aReason)
{
	int strLen	= strlen("Couldn't load:\t") + strlen(aFile) + strlen("\nReason:\t") + strlen(aReason);
	char *msg	= (char*)malloc(strLen);
	memset		(msg, 0, strLen);

	strcat(msg, "Couldn't load:\t");
	strcat(msg, aFile);
	strcat(msg, "\nReason:\t");
	strcat(msg, aReason);
	this->msg = msg;
}
