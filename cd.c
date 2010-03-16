/*
 * cd.c
 *
 *  Created on: 16/03/2010
 *      Author: Dennis Hedegaard
 */

#include "cd.h"

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
	if (chdir(relativedir) != 0) {
		switch (errno) {
		case EACCES:
			fprintf(stderr, "You don't have the proper permissions for: %s\n",
					relativedir);
			break;
		case ELOOP:
			fprintf(stderr, "There's a symlink loop in the path: %s\n",
					relativedir);
			break;
		case ENAMETOOLONG:
			fprintf(
					stderr,
					"The path length exceeds the c implementations maximum: %s\n",
					relativedir);
			break;
		case ENOENT:
			fprintf(stderr,
					"The path specified does not name a directory : %s\n",
					relativedir);
			break;
		case ENOTDIR:
			fprintf(stderr, "Parts of the path is not a directory: %s\n",
					relativedir);
			break;
		default:
			fprintf(stderr,
					"unknown error, when trying to change dir to: %s\n",
					relativedir);
			break;
		}
	}
	if (shouldfree && relativedir != NULL)
		free(relativedir);
}
