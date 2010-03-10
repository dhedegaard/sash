/*
 * environ.h
 *
 *  Created on: 10/03/2010
 *      Author: Dennis Hedegaard
 */

#include "environ.h"

char **_env;
int _envlen;

void setenviron(char **env) {
	int i;
	_env = env;
	while (_env[i] != NULL)
		i++;
}
