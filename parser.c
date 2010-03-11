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
		quit();
	else if (strcmp("help", trimmed) == 0)
		help();
	else if (startswith(trimmed, "echo ") == 0) {
		char *toecho = removefirstword(trimmed);
		echo(toecho);
		free(toecho);
	} else if (strcmp("environ", trimmed) == 0)
		environ();
	else if (strcmp("ls", trimmed) == 0)
		ls(trimmed);
	else if (strcmp("cd", trimmed) == 0)
		cd(trimmed);
	else
		cmd_exec(trimmed);
	free(trimmed);
}
