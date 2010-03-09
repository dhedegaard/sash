/*
 * commmand.c
 *
 *  Created on: 08/03/2010
 *      Author: Dennis Hedegaard
 */

#include "command.h"

void help() {
	FILE *f = fopen("readme", "r");
	if (f == NULL) {
		fprintf(stderr, "Unable to find readme.\n");
	}
	else {
		char c, prior;
		while ((c = fgetc(f)) != EOF) {
			printf("%c", c);
			prior = c;
		}
		if (prior != '\n');
			printf("\n");

	}
	fclose(f);
}

void quit() {
	printf("Quttting..\n");
	exit(0);
}
