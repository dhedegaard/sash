/*
 * commmand.c
 *
 *  Created on: 08/03/2010
 *      Author: Dennis Hedegaard
 */

#include "command.h"

void cmd_help() {
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

void cmd_quit() {
	printf("Qutting..\n");
	exit(0);
}

void cmd_cd(const char *dir) {
	/* TODO implement */
	fprintf(stderr, "TODO\n");
}

void cmd_ls(const char *dir) {
	ls_ls(dir);
}

void cmd_environ() {
	getenviron(stdout, fprintf);
}

void cmd_echo(const char *str) {
	if (str == NULL)
		fprintf(stderr, "echo: not arguments supplied.\n");
	else
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

