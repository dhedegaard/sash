/*
 * environ.h
 *
 *  Created on: 10/03/2010
 *      Author: Dennis Hedegaard
 */

#include "environ.h"

/**
 * Returns the value after equals in a string, for instance
 * if you want the value from a key=value string.
 */
char *getvalueafterequals(const char *string);

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
	/* untested */
	int i;
	if (key == NULL)
		return NULL;
	for (i = 0; i < _envlen; i++)
		if (startswith(_env[i], key))
			return getvalueafterequals(_env[i]);
	return NULL;
}

void getenviron(FILE* stream, int(*f)(FILE*, const char*, ...)) {
	int i;
	f(stream, "Environmental values:\n");
	for (i = 0; i < _envlen; i++)
		f(stream, "%s\n", _env[i]);
}

char *getvalueafterequals(const char *string) {
	/* untested */
	int i, len = strlen(string), pos = -1;
	char *value;
	for (i = 0;i < len;i++)
		if (string[i] == '=') {
			pos = i + 1;
			break;
		}
	if (pos == -1)
		return NULL;
	value = malloc(sizeof(*value) * (len - pos + 1));
	memcpy(value, string, len - pos + 1);
	return value;
}
