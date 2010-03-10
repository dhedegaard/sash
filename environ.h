/*
 * environ.c
 *
 *  Created on: 10/03/2010
 *      Author: Dennis Hedegaard
 */

#ifndef ENVIRON_H_
#define ENVIRON_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utility.h"

/**
 * Sets the env variable, this is used later for "querying".
 */
void setenviron(char **env);
/**
 * gets a specific environ value. If the value is not found,
 * NULL is returned.
 * remember to free the pointer, when you're done with it.
 */
const char *getenvironvalue(const char *key);
/**
 * Takes all environmental values and puts the through the
 * stream with a function like fprintf.
 * Perhaps a better implementation could be useful here.
 */
void getenviron(FILE* stream, int(*f)(FILE*, const char*, ...));

#endif /* ENVIRON_H_ */
