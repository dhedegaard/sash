/*
 * ls.c
 *
 *  Created on: 11/03/2010
 *      Author: Dennis Hedegaard
 */

#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "stack.h"
#include "utility.h"
#include "ls.h"

/**
 * A callback function, that should implement a simple way to
 * compare c string, for sorting out the dirs.
 */
static int compare_chars(const void *a, const void *b);
/**
 * Call if readdir() fails and errno is different from before
 * the call, then something bad happened.
 */
static void handle_readdir_err();

void ls_ls(const char *_dir) {
	DIR *d;
	struct dirent *dir;
	int maxlen;
	if ((d = opendir(_dir == NULL ? "." : _dir)) != 0) {
		struct _stack_t *stack;
		char **arr, **parr;
		stack = openstack(stack);
		/* push all the directory names onto a stack. */
		{
			int olderr = errno;
			while ((dir = readdir(d)) != NULL)
				push(stack, dir->d_name);
			if (errno != olderr) {
				handle_readdir_err();
				closedir(d);
				if (stack != NULL)
					closestack(stack);
				return;
			}
		}
		maxlen = stack_size(stack);
		arr = calloc(maxlen + 1, sizeof(*arr));
		/* pop the stack onto a fixed size array. */
		parr = arr;
		while (stack_size(stack) > 0)
			*parr++ = pop(stack);
		*parr = NULL;
		/* sort and print the array */
		qsort(arr, maxlen, sizeof(*arr), compare_chars);
		/* for (pos = 0; pos < maxlen; pos++)
		 printf("%s\n", arr[pos]); */
		parr = arr;
		while (*parr != NULL) {
			printf("%s\n", *parr);
			free(*parr++);
		}
		/* cleanup after ourselves. */
		closedir(d);
		closestack(stack);
		free(arr);
	} else
		fprintf(stderr, "sash: %s: %s\n", _dir == NULL ? "." : _dir, strerror(
				errno));
}

static int compare_chars(const void *a, const void *b) {
	const char **ia = (const char **) a, **ib = (const char **) b;
	return strcmp(*ia, *ib);
}

static void handle_readdir_err() {
	fprintf(stderr, "%s\n", strerror(errno));
	/*switch (errno) {
	 case EOVERFLOW:
	 fprintf(stderr, "One of the values in the directory,"
	 " cannot be represented correctly.\n");
	 break;
	 case EBADF:
	 fprintf(stderr, "the directory stream is not open.\n");
	 break;
	 case ENOENT:
	 fprintf(stderr, "the current position in the directory"
	 "stream is invalid.\n");
	 break;
	 default:
	 fprintf(stderr, "an unknown error was encountered while"
	 " trying to get directories.\n");
	 break;
	 }*/
}
