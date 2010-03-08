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
	else if(strcmp("help", trimmed) == 0)
		help();
	else
		BAD_COMMAND(trimmed);
	free(trimmed);
}
