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
			;
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
	fprintf(stderr, "TODO");
}

void ls(const char *dir) {
	/* TODO implement */
	fprintf(stderr, "TODO");
}

void environ() {
	/* TODO implement */
	fprintf(stderr, "TODO");
}

void echo(const char *str) {
	printf("%s\n", str);
}

void cmd_exec(const char *cmd) {
	pid_t child, parent;
	parent = getpid();
	child = fork();
	if (child < 0) {
		fprintf(stderr, "command.c:cmd_exec(): Failed to fork process.\n");
		exit(1);
	}
	if (child == getpid()) {
		/* int res = system(cmd); */
		int res = execv(cmd, (char *) NULL);
		printf("res: %d\n", res);
		exit(res);
	}
}

