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
	size_t size = 17, maxpath = MAX_LINE_LENGTH;
	pid = malloc(sizeof(*pid) * size + 1);
	if (snprintf(pid, size, "/proc//exe") <= 0) {
		fprintf(stderr, "Unable to get pid, in less than %lu chars.\n", size);
		exit(1);
	}
	newshell = malloc(sizeof(*newshell) * maxpath + 1);
	if (readlink(pid, newshell, maxpath) == -1) {
		fprintf(stderr, "warning: Unable to read executable path from: %s\n",
				pid);
	}
	if (setenv("SHELL", newshell, 1) == -1) {
		if (errno == ENOMEM)
			fprintf(stderr, "warning: no memory available for putenv().\n");
		else
			fprintf(stderr,
					"warning: Unable to set the shell for unknown reasons.\n");
	}
	free(pid);
	free(newshell);
}
