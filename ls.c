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

void ls_ls(const char *_dir) {
	DIR *d;
	struct dirent *dir;
	int size = 0;
	if ((d = opendir(_dir == NULL ? "." : _dir)) != 0) {
		stack_t stack;
		int pos = 0;
		const char **arr, *e;
		stack.top = NULL;
		/* push all the directory names onto a stack. */
		while ((dir = readdir(d)) != NULL) {
			push(&stack, dir->d_name);
			size++;
		}
		closedir(d);
		arr = malloc(sizeof(*arr) * size);
		/* pop the stack, until it's empty. */
		while ((e = pop(&stack)) != NULL)
			arr[pos++] = e;
		qsort(arr, size, sizeof(*arr), compare_chars);
		for (pos = 0; pos < size; pos++)
			printf("%s\n", arr[pos]);
		free(arr);
	} else {
		const char *errdir = _dir == NULL ? "." : _dir;
		switch (errno) {
		case EACCES:
			fprintf(stderr, "You don't have the proper permissions for: %s\n",
					errdir);
			break;
		case ELOOP:
			fprintf(stderr, "There's a symlink loop in the path: %s\n", errdir);
			break;
		case ENAMETOOLONG:
			fprintf(
					stderr,
					"The path length exceeds the c implementations maximum: %s\n",
					errdir);
			break;
		case ENOENT:
			fprintf(stderr,
					"The path specified does not name a directory : %s\n",
					errdir);
			break;
		case ENOTDIR:
			fprintf(stderr, "Parts of the path is not a directory: %s\n",
					errdir);
			break;
		default:
			fprintf(stderr, "There's no such directory: %s\n", errdir);
			break;
		}
	}
}

static int compare_chars(const void *a, const void *b) {
	const char **ia = (const char **) a, **ib = (const char **) b;
	return strcmp(*ia, *ib);
}
