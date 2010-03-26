/*
 * prompt.c
 *
 *  Created on: 17/03/2010
 *      Author: Dennis Hedegaard
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#include "utility.h"
#include "prompt.h"

/**
 * Checks to see if dir starts with the homedir, if it does,
 * then the size of the homedir is returned, otherwise 0 is returned.
 */
static int startswithhomedir(const char *dir);

void printprompt() {
	char cmd[PATH_MAX];
	char *pcmd = cmd;
	int cmdlen;
	if ((pcmd = getcwd(pcmd, PATH_MAX)) == NULL)
		fprintf(stderr, "%s\n", strerror(errno));
	else {
		int len;
		cmdlen = strlen(cmd);
		if ((len = startswithhomedir(cmd)) > 0) {
			char *pcmd = cmd;
			printf("~");
			if (len > cmdlen)
				printf("/");
			printf("%s", (pcmd + len));
		} else
			printf("%s", cmd);
	}
	printf(" $ ");
}

static int startswithhomedir(const char *dir) {
	static char *home;
	if (home == NULL)
		home = getenv("HOME");
	if (home == NULL)
		return 0;
	if (startswith(dir, home))
		return strlen(home);
	else
		return 0;
}
