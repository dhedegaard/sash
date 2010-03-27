/*
 * exec.c
 *
 *  Created on: 17/03/2010
 *      Author: Dennis Hedegaard
 */

#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/wait.h>
#include <stdio.h>

#include "exec.h"

static int execiopipe(arg_t *args);

int exec(arg_t *args) {
	execiopipe(args);
	return 0;
}

static int execiopipe(arg_t *args) {
	int res = 0, childpid;
	FILE *fin = NULL, *fout = NULL;
	switch (childpid = fork()) {
	case -1:
		strerror(errno);
		return errno;
		break;
	case 0:
		if (args->inputfile != NULL) {
			if ((fin = freopen(args->inputfile, "r", stdin)) == NULL) {
				strerror(errno);
				_exit(errno);
			}
		}
		if (args->outputfile != NULL) {
			if ((fout = freopen(args->outputfile, "a", stdout)) == NULL) {
				strerror(errno);
				_exit(errno);
			}
		}
		res = execvp(args->argv[0], args->argv);
		if (res == -1) {
			char *err;
			if ((err = strerror(errno)) == NULL)
				fprintf(stderr, "%s: unknown error, errno: %d.\n",
						args->argv[0], errno);
			else
				fprintf(stderr, "%s: %s\n", args->argv[0], err);
		}
		if (fin)
			fclose(fin);
		if (fout)
			fclose(fout);
		exit(res);
		break;
	default:
		waitpid(childpid, &res, 0);
		break;
	}
	return res;
}
