/*
 * parser.c
 *
 *  Created on: 08/03/2010
 *      Author: Dennis Hedegaard
 */

#include "parser.h"

void parse(const char *input) {
	char *trimmed = trim(input);
	if (strcmp("quit", trimmed) == 0)
		cmd_quit();
	else if (strcmp("help", trimmed) == 0)
		cmd_help();
	else if (startswith(trimmed, "echo ") == 0) {
		char *toecho = removefirstword(trimmed);
		cmd_echo(toecho);
		free(toecho);
	} else if (strcmp("environ", trimmed) == 0)
		cmd_environ();
	else if (startswith(trimmed, "ls") == 0) {
		char *dir = removefirstword(trimmed);
		cmd_ls(dir);
		if (dir != NULL)
			free(dir);
	} else if (strcmp("cd", trimmed) == 0)
		cmd_cd(trimmed);
	else
		cmd_exec(trimmed);
	free(trimmed);
}
