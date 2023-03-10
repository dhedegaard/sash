/*
 * commmand.c
 *
 *  Created on: 08/03/2010
 *      Author: Dennis Hedegaard
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "command.h"
#include "utility.h"
#include "environ.h"
#include "ls.h"
#include "cd.h"
#include "exec.h"

void cmd_help() {
	char *helpfile = malloc(PATH_MAX);
	FILE *f;
	if (snprintf(helpfile, PATH_MAX, "%s/readme", getstartwd()) < 0) {
		fprintf(stderr, "command.c:sprintf(): error writing to buffer.\n");
		exit(1);
	}
	if ((f = fopen(helpfile, "r")) == NULL) {
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
	if (f)
		fclose(f);
	if (helpfile)
		free(helpfile);
}

void cmd_quit() {
	printf("Qutting..\n");
	environ_cleanup();
	exit(0);
}

void cmd_cd(const char *dir) {
	cd_cd(dir);
}

void cmd_ls(const char *dir) {
	char *path = getrelativepath(dir);
	ls_ls(path);
	free(path);
}

void cmd_environ() {
	getenviron(stdout, fprintf);
}

void cmd_echo(const char *str) {
	fprintf(stderr, "using deprecated function cmd_echo(const char*)");
	if (str == NULL)
		fprintf(stderr, "echo: not arguments supplied.\n");
	else
		printf("%s\n", str);
}

void cmd_exec(struct arg_t *args) {
	exec(args);
}

void cmd_clear() {
	printf("\033[2J\033[H");
}
