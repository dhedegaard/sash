/*
 * utility.h
 *
 *  Created on: 08/03/2010
 *      Author: Dennis Hedegaard
 */

#ifndef UTILITY_H_
#define UTILITY_H_

/**
 * Does a simple trim of a string, if the string is NULL or
 * everything is spaces and tabs, NULL is returned. Otherwise
 * a trimmed version of the string is returned.
 * Important: Please remember to free the string again, since
 * it's allocated with malloc.
 */
char* trim(const char*);
/**
 * Checks to see if "string" starts with the same characters as
 * "startwith" contains. If startwith is larger than string, 1
 * is returned.
 */
int startswith(const char*, const char*);
/**
 * Attempts to remove the first word from the string, this means
 * everything before the first space, including the space.
 * Keep in mind though, that the char * must be freed again to avoid
 * leaks.
 * This function does not handle trimming, this may or may not cause
 * unexpected problems.
 */
char* removefirstword(const char*);
/**
 * Does a simple parsing of a relative path, changing different weird
 * symbols input read path data.
 * Remember to free the result eventually, to avoid leaks.
 */
char* getrelativepath(const char*);

#endif /* UTILITY_H_ */
