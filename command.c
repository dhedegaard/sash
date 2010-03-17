/*
 * commmand.c
 *
 *  Created on: 08/03/2010
 *      Author: Dennis Hedegaard
 */

#include <stdio.h>
#include <stdlib.h>

#include "command.h"
#include "global.h"
#include "utility.h"
#include "environ.h"
#include "ls.h"
#include "cd.h"
#include "exec.h"

void cmd_help() {
	char *helpfile = malloc(sizeof(*helpfile) * MAX_LINE_LENGTH);
	FILE *f;
	if (sprintf(helpfile, "%s/readme", getstartwd()) < 0) {
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
	fclose(f);
	free(helpfile);
}

void cmd_quit() {
	printf("Qutting..\n");
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
	if (str == NULL)
		fprintf(stderr, "echo: not arguments supplied.\n");
	else
		printf("%s\n", str);
}

void cmd_exec(const char *cmd) {
	/* pid_t child;
	child = fork();
	if (child < 0) { / * if unable to fork child. */
		/*fprintf(stderr, "command.c:cmd_exec(): Failed to fork process.\n");
		exit(1);
	} else if (child == 0) { / * if is child. */
		/*int result = system(cmd);
		printf("child (%d) returned: %d\n", getpid(), result);
		_exit(result);
	} */
	exec(cmd);
}

