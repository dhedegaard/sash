/*
 * ls.c
 *
 *  Created on: 11/03/2010
 *      Author: Dennis Hedegaard
 */

#include "ls.h"

void ls_ls(const char *_dir) {
	DIR *d;
	struct dirent *dir;
	if ((d = opendir(_dir == NULL ? "." : _dir)) != 0) {
		while ((dir = readdir(d)) != NULL)
			printf("%s\n", dir->d_name);
		closedir(d);
	}
}
