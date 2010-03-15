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

struct node {
	struct node *next;
	char *name;
};

void ls_ls(const char *_dir) {
	DIR *d;
	struct dirent *dir;
	int size = 0;
	struct node *top = NULL;
	if ((d = opendir(_dir == NULL ? "." : _dir)) != 0) {
		int pos;
		char **arr;
		while ((dir = readdir(d)) != NULL) {
			struct node *newnode = malloc(sizeof(*newnode));
			newnode->name = dir->d_name;
			newnode->next = top;
			top = newnode;
			size++;
		}
		closedir(d);
		pos = 0;
		arr = malloc(sizeof(*arr) * size);
		while (top != NULL) {
			struct node *oldtop = top;
			arr[pos++] = top->name;
			top = top->next;
			free(oldtop);
		}
		qsort(arr, size, sizeof(*arr), compare_chars);
		for (pos = 0; pos < size; pos++)
			printf("%s\n", arr[pos]);
		free(arr);
	} else
		fprintf(stderr, "There's no such directory: %s\n", _dir);
}

static int compare_chars(const void *a, const void *b) {
	const char **ia = (const char **) a, **ib = (const char **) b;
	return strcmp(*ia, *ib);
}
