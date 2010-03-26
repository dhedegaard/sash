/*
 * main.c
 *
 *  Created on: 08/03/2010
 *      Author: Dennis Hedegaard
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> /* readlink */
#include <errno.h>
#include <inttypes.h>
#include <limits.h>

#include "utility.h"
#include "parser.h"
#include "environ.h"
#include "prompt.h"
#include "main.h"

/**
 * Attemps to set the SHELL environmental variable, using
 * getcwd() and setenv().
 */
static void setshellpath();

int main(int argc, char **argv, char **env) {
	char input[LINE_MAX];
	printf("sash shell revision: %d\n", 1);
	setshellpath();
	setenviron(env);
	while (1) {
		printprompt();
		if (fgets(input, LINE_MAX, stdin) == NULL) {
			fprintf(stderr, "\nEOF caught, quitting.\n");
			exit(1);
		}
		parse(input);
	}
	environ_cleanup();
	return 0;
}

static void setshellpath() {
	char *pid = NULL, *newshell = NULL;
	pid = malloc(sizeof(*pid) * (PATH_MAX + 1));
	if (snprintf(pid, PATH_MAX, "/proc/%d/exe", getpid()) <= 0) {
		fprintf(stderr, "error: unable to get pid, in less than %d chars.\n",
				PATH_MAX);
		exit(1);
	}
	newshell = calloc(sizeof(*newshell), PATH_MAX + 1);
	if (readlink(pid, newshell, PATH_MAX) == -1) {
		fprintf(stderr, "setshellpath: %s\n", strerror(errno));
		return;
	}
	if (setenv("SHELL", newshell, 1) == -1) {
		if (errno == EINVAL)
			fprintf(stderr,
					"warning: the shell argument pointer either starts with"
						" a '=', is null or points to an empty string.\n");
		else if (errno == ENOMEM)
			fprintf(stderr, "warning: no memory available for putenv().\n");
		else
			fprintf(stderr,
					"warning: Unable to set the shell for unknown reasons.\n");
	}
	if (pid)
		free(pid);
	if (newshell)
		free(newshell);
}
