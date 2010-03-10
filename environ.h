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
 */
const char *getenvironvalue(const char *key);
/**
 * gets all specified environ values, each on a line with the
 * key=value format. Each line is called to the function f.
 */
void getenviron(FILE* stream, void(*f)(FILE*, const char*, ...));

#endif /* ENVIRON_H_ */
