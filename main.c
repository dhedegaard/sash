/*
 * main.c
 *
 *  Created on: 08/03/2010
 *      Author: Dennis Hedegaard
 */

#include "main.h"

/**
 * Attemps to set the SHELL environmental variable, using
 * getcwd() and setenv().
 */
void setshellpath(char *arg0);

int main(int argc, char **argv, char **env) {
	char input[MAX_LINE_LENGTH];
	/* setshellpath(argv[0]); */
	setenviron(env);
	printf("%s\n", getenv("test"));
	while (1) {
		printf("%s", PROMPT);
		if (fgets(input, MAX_LINE_LENGTH, stdin) == NULL) {
			fprintf(stderr, "There was an error reading from stdin.\n");
			exit(1);
		}
		parse(input);
	}
	return 0;
}

void setshellpath(char *arg0) {
	/* TODO fix */
/*	char *c, *cwd = getcwd(NULL, 0);
	int len = strlen(cwd) + strlen(arg0) + 1 + 4, i = 4;
	c = malloc(sizeof(*c) * len);
	memcpy(c, "PWD=", 4);
	setenv("test=test");
	free(c);*/
}
