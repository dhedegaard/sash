/*
 * cd.c
 *
 *  Created on: 16/03/2010
 *      Author: Dennis Hedegaard
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "utility.h"

void cd_cd(const char *dir) {
	int shouldfree;
	char *relativedir;
	if (dir == NULL) {
		shouldfree = 0;
		relativedir = getenv("HOME");
	} else {
		shouldfree = 1;
		relativedir = getrelativepath(dir);
	}
	if (chdir(relativedir) != 0)
		fprintf(stderr, "sash: cd: %s: %s\n", strerror(errno), dir);
	if (shouldfree && relativedir != NULL)
		free(relativedir);
}
