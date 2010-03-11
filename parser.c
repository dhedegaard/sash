/*
 * parser.c
 *
 *  Created on: 08/03/2010
 *      Author: Dennis Hedegaard
 */

#include "parser.h"

void parse(const char *input) {
	char *trimmed;
	trimmed = trim(input);
	if (strcmp("quit", trimmed) == 0 || strcmp("exit", trimmed) == 0)
		cmd_quit();
	else if (strcmp("help", trimmed) == 0)
		cmd_help();
	else if (startswith(trimmed, "echo ") == 0) {
		cmd_echo(removefirstword(trimmed));
	} else if (strcmp("environ", trimmed) == 0)
		cmd_environ();
	else if (startswith(trimmed, "ls") == 0 || startswith(trimmed, "dir") == 0)
		cmd_ls(removefirstword(trimmed));
	else if (startswith(trimmed, "cd ") == 0)
		cmd_cd(removefirstword(trimmed));
	else
		cmd_exec(trimmed);
	free(trimmed);
}
