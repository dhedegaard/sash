/*
 * prompt.c
 *
 *  Created on: 17/03/2010
 *      Author: neo2k
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "global.h"
#include "utility.h"
#include "prompt.h"

/**
 * Checks to see if dir starts with the homedir, if it does,
 * then the size of the homedir is returned, otherwise 0 is returned.
 */
/*static int startswithhomedir(const char *dir);*/

void printprompt() {
	/* char cmd[MAX_LINE_LENGTH];
	if (getcwd(cmd, MAX_LINE_LENGTH) == NULL)
		switch (errno) {
		case ENOMEM:
			fprintf(stderr, "Not enough memory to get current path.\n");
			break;
		case ERANGE:
			fprintf(stderr, "The path length is above %d\n", MAX_LINE_LENGTH);
			break;
		case EACCES:
			fprintf(stderr,
					"Permissions denied when trying to get the work dir.\n");
			break;
		default:
			fprintf(stderr, "Unknwon error trying to get current workdir.\n");
			break;
		}
	else {
		int i, len;
		char *prompt;
		if ((len = startswithhomedir(cmd)) > 0) {
			int newlen;
			newlen = 1 - len + strlen(cmd);
			if (newlen > 1) {
				prompt = malloc(sizeof(*prompt) * (newlen + 4));
				prompt[0] = '~';
				prompt[1] = '/';
				for (i = 2; i < newlen; i++)
					prompt[i] = cmd[i + len - 1];
			}
			else {
				prompt = "TODO:";
			}
		} else {
			len = strlen(cmd);
			prompt = malloc(sizeof(*prompt) * (len + 4));
			for (i = 0; i < len; i++)
				prompt[i] = cmd[i];
			prompt[len] = ' ';
			prompt[len + 1] = '$';
			prompt[len + 2] = ' ';
			prompt[len + 3] = '\0';
		}
		printf("%s", prompt);
		free(prompt);
	}*/
	printf("$ ");
}

/*static int startswithhomedir(const char *dir) {
	static char *home;
	if (home == NULL)
		getenv("HOME");
	return home == NULL ? 0 : startswith(dir, home);
}*/
