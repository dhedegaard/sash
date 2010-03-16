/*
 * environ.h
 *
 *  Created on: 10/03/2010
 *      Author: Dennis Hedegaard
 */

#include "environ.h"

/**
 * Returns the value before equals in a string, this is
 * nice for getting the key from a key=value string.
 */
static char *getvaluebeforeequals(const char *string);

static int _envlen;
static char **_env;
static char *_startwd;

void setenviron(char **env) {
	_envlen = 0;
	_env = env;
	while (env[_envlen] != NULL)
		_envlen++;
	_startwd = getcwd(NULL, 0);
}

void getenviron(FILE* stream, int(*f)(FILE*, const char*, ...)) {
	int i;
	if (_env == NULL) {
		f(stream, "environ.c:getenviron(): You forgot to run setenviron()!\n");
		return;
	}
	f(stream, "Environmental values:\n");
	for (i = 0; i < _envlen; i++) {
		char *key = getvaluebeforeequals(_env[i]);
		f(stream, "%s: %s\n", key, getenv(key));
		free(key);
		key = NULL;
	}
}

static char *getvaluebeforeequals(const char *string) {
	int len = strlen(string), pos;
	char *value;
	for (pos = 0; pos < len; pos++)
		if (string[pos] == '=')
			break;
	if (pos >= len || pos == 0)
		return NULL;
	value = malloc(sizeof(*value) * (pos + 1));
	memcpy(value, string, pos);
	value[sizeof(*value) * (pos)] = '\0';
	return value;
}

const char *getstartwd() {
	return _startwd;
}
