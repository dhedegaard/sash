/*
 * utility.h
 *
 *  Created on: 08/03/2010
 *      Author: Dennis Hedegaard
 */

#ifndef UTILITY_H_
#define UTILITY_H_

#include <string.h>
#include <stdlib.h>

/**
 * Does a simple trim of a string, if the string is NULL or
 * everything is spaces and tabs, NULL is returned. Otherwise
 * a trimmed version of the string is returned.
 * Important: Please remember to free the string again, since
 * it's allocated with malloc.
 */
char* trim(const char *string);

#endif /* UTILITY_H_ */
