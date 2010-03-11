/*
 * commmand.c
 *
 *  Created on: 08/03/2010
 *      Author: Dennis Hedegaard
 */

#include "command.h"

void help() {
	FILE *f = fopen("readme", "r");
	if (f == NULL) {
		fprintf(stderr, "Unable to find readme.\n");
	} else {
		char c, prior;
		while ((c = fgetc(f)) != EOF) {
			printf("%c", c);
			prior = c;
		}
		if (prior != '\n')
			printf("\n");
	}
	fclose(f);
}

void quit() {
	printf("Qutting..\n");
	exit(0);
}

void cd(const char *dir) {
	/* TODO implement */
	fprintf(stderr, "TODO\n");
}

void ls(const char *dir) {
	/* TODO implement */
	fprintf(stderr, "TODO\n");
}

void environ() {
	getenviron(stdout, fprintf);
}

void echo(const char *str) {
	printf("%s\n", str);
}

void cmd_exec(const char *cmd) {
	pid_t child;
	child = fork();
	if (child < 0) { /* if unable to fork child. */
		fprintf(stderr, "command.c:cmd_exec(): Failed to fork process.\n");
		exit(1);
	} else if (child == 0) { /* if is child. */
		int result = system(cmd);
		printf("child (%d) returned: %d\n", getpid(), result);
		exit(result);
	}
}

