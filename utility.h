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
/**
 * Checks to see if "string" starts with the same characters as
 * "startwith" contains. If startwith is larger than string, 1
 * is returned.
 */
int startswith(const char *string, const char *startwith);
/**
 * Attempts to remove the first word from the string, this means
 * everything before the first space, including the space.
 * Keep in mind though, that the char * must be freed again to avoid
 * leaks.
 * This function does not handle trimming, this may or may not cause
 * unexpected problems.
 */
char *removefirstword(const char *string);
/**
 * This function takes a string, and splits it up whenever a space is
 * encountered in the string, the external pointer is null terminated.
 * If the string supplied is NULL, NULL is returned.
 */
char **splitonspace(const char *str);

#endif /* UTILITY_H_ */
