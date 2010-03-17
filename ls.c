/*
 * ls.c
 *
 *  Created on: 11/03/2010
 *      Author: Dennis Hedegaard
 */

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
		stack_t *stack = malloc(sizeof(*stack));
		int pos = 0;
		const char **arr;
		stack->top = NULL;
		stack->size = 0;
		/* push all the directory names onto a stack. */
		{
			int olderr = errno;
			while ((dir = readdir(d)) != NULL)
				push(stack, dir->d_name);
			if (errno != olderr) {
				handle_readdir_err();
				closedir(d);
				return;
			}
		}
		maxlen = stack->size;
		arr = malloc(sizeof(*arr) * maxlen);
		/* pop the stack, until it's empty. */
		while (stack->size > 0 && stack->top != NULL)
			arr[pos++] = pop(stack);
		qsort(arr, maxlen, sizeof(*arr), compare_chars);
		for (pos = 0; pos < maxlen; pos++)
			printf("%s\n", arr[pos]);
		closedir(d);
		free(arr);
		free(stack);
	} else
		patherrorhandling(_dir == NULL ? "." : _dir);
}

static int compare_chars(const void *a, const void *b) {
	const char **ia = (const char **) a, **ib = (const char **) b;
	return strcmp(*ia, *ib);
}

static void handle_readdir_err() {
	switch (errno) {
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
	}
}
