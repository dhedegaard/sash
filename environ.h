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

/**
 * Sets the env variable, this is used later for "querying".
 */
void setenviron(char **env);

#endif /* ENVIRON_H_ */
