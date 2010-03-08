/*
 * main.c
 *
 *  Created on: 08/03/2010
 *      Author: Dennis Hedegaard
 */

#include "main.h"

int main(int argc, char **argv) {
	char input[MAX_LINE_LENGTH];
	while (1) {
		printf("%s", PROMPT);
		if (fgets(input, MAX_LINE_LENGTH, stdin) == NULL) {
			fprintf(stderr, "There was an error reading from stdin.\n");
			exit(1);
		}
		parse(input);
	}
	return 0;
}
