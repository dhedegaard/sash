/*
 * environ.h
 *
 *  Created on: 10/03/2010
 *      Author: Dennis Hedegaard
 */

#include "environ.h"

typedef struct _keyvalue {
	const char *key;
	const char *value;
} keyvalue;

/**
 * Returns the value before equals in a string, this is
 * nice for getting the key from a key=value string.
 */
char *getvaluebeforeequals(const char *string);

int _envlen;
char **_env;

void setenviron(char **env) {
	_envlen = 0;
	_env = env;
	while (env[_envlen] != NULL)
		_envlen++;
}

void getenviron(FILE* stream, int(*f)(FILE*, const char*, ...)) {
	int i;
	f(stream, "Environmental values:\n");
	for (i = 0; i < _envlen; i++) {
		char *key = getvaluebeforeequals(_env[i]);
		f(stream, "%s: %s\n", key, getenv(key));
		free(key);
		key = NULL;
	}
}

char *getvaluebeforeequals(const char *string) {
	int len = strlen(string), pos;
	char *value;
	for (pos = 0;pos < len;pos++)
		if (string[pos] == '=')
			break;
	if (pos >= len || pos == 0)
		return NULL;
	value = malloc(sizeof(*value) * (pos + 1));
	memcpy(value, string, pos);
	value[sizeof(*value) * (pos)] = '\0';
	return value;
}
