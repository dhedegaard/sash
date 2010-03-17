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
static void setshellpath();

int main(int argc, char **argv, char **env) {
	char input[MAX_LINE_LENGTH];
	printf("sash shell revision: %d\n", REVISION);
	setshellpath();
	setenviron(env);
	while (1) {
		printprompt();
		if (fgets(input, MAX_LINE_LENGTH, stdin) == NULL) {
			fprintf(stderr, "\nEOF caught, quitting.\n");
			exit(1);
		}
		parse(input);
	}
	return 0;
}

static void setshellpath() {
	char *pid, *newshell;
	pid = malloc(sizeof(*pid) * (PATH_MAX + 1));
	if (snprintf(pid, PATH_MAX, "/proc/%d/exe", getpid()) <= 0) {
		fprintf(stderr, "error: unable to get pid, in less than %d chars.\n",
				PATH_MAX);
		exit(1);
	}
	newshell = malloc(sizeof(*newshell) * (PATH_MAX + 1));
	if (readlink(pid, newshell, PATH_MAX) == -1) {
		const char *err;
		switch (errno) {
		case EACCES:
			err = "search or read permissions denied for path";
			break;
		case EINVAL:
			err = "the path name is not a symlink";
			break;
		case EIO:
			err = "an IO error occured for";
			break;
		case ELOOP:
			err = "there's a loop in the symlink chain for";
			break;
		case ENAMETOOLONG:
			err = "the path is too long";
			break;
		case ENOENT:
			err = "the link does not exist";
			break;
		case ENOTDIR:
			err = "a part of the path is not a directory";
			break;
		default:
			err = "unknown error, unable to access";
			break;
		}
		fprintf(stderr, "warning: %s: %s\n", err, pid);
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
	free(pid);
	free(newshell);
}
