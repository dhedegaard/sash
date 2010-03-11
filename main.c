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
void setshellpath();

int main(int argc, char **argv, char **env) {
	char input[MAX_LINE_LENGTH];
	printf("sash shell revision: %d\n", REVISION);
	setshellpath();
	setenviron(env);
	while (1) {
		printf("%s", PROMPT);
		if (fgets(input, MAX_LINE_LENGTH, stdin) == NULL) {
			fprintf(stderr, "\nEOF caught, quitting.\n");
			exit(1);
		}
		parse(input);
	}
	return 0;
}

void setshellpath() {
	char *pid, *newshell;
	pid = malloc(sizeof(*pid) * 17);
	if (snprintf(pid, 16, "/proc/%d/exe", getpid()) < 0) {
		fprintf(stderr,
				"Unable to get pid, in less than 7 (16 for the rest) chars.\n");
		exit(1);
	}
	printf("pid: %s\n", pid);
	newshell = malloc(sizeof(*newshell) * 255);
	if (readlink(pid, newshell, 254) == -1) {
		fprintf(stderr, "Unable to read executable path from: %s\n", pid);
		exit(1);
	}
	if (setenv("SHELL", newshell, 1) == -1) {
		fprintf(stderr, "Unable to set the shell to: %s\n", newshell);
	}
	free(pid);
	free(newshell);
}
