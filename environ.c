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
	_envlen = i;
}

const char *getenvironvalue(const char *key) {
	/* TODO implement */
	return NULL;
}

void getenviron(FILE* stream, int(*f)(FILE*, const char*, ...)) {
	int i;
	f(stream, "Environmental values:\n");
	for (i = 0; i < _envlen; i++)
		f(stream, "%s\n", _env[i]);
}
