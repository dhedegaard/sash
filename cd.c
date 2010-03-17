/*
 * cd.c
 *
 *  Created on: 16/03/2010
 *      Author: Dennis Hedegaard
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "cd.h"
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
		patherrorhandling(relativedir);
	if (shouldfree && relativedir != NULL)
		free(relativedir);
}
