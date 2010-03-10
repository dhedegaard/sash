/*
 * commmand.c
 *
 *  Created on: 08/03/2010
 *      Author: Dennis Hedegaard
 */

#include "command.h"

void help() {
	printf("TODO\n");
}

void quit() {
	printf("Qutting..\n");
	exit(0);
}

void cd(const char *dir) {
	/* TODO implement */
	fprintf(stderr, "TODO");
}

void ls(const char *dir) {
	/* TODO implement */
	fprintf(stderr, "TODO");
}

void environ() {
	/* TODO implement */
	fprintf(stderr, "TODO");
}

void echo(const char *str) {
	printf("%s\n", str);
}
