/*
 * utility.c
 *
 *  Created on: 08/03/2010
 *      Author: Dennis Hedegaard
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

#include "utility.h"

/**
 * Allocates and makes a new string based on the input from trim.
 * It always allocates 1 extra byte, for terminating the string (\0).
 */
static char* makestring(int left, int right, const char *string);
/**
 * Checks to see if a char is "valid", meaning it shouldn't get trimmed.
 */
static int checkvalid(char c);

char* trim(const char *string) {
	int left, right = 0, i;
	if (string == NULL)
		return NULL;
	left = 0;
	right = strlen(string) - 1;
	for (i = 0; i <= right; i++)
		if (!checkvalid(string[i]))
			left = i;
		else
			break;
	for (i = right; i >= left; i--)
		if (!checkvalid(string[i]))
			right = i;
		else
			break;
	if (left >= right)
		return NULL;
	else
		return makestring(left, right, string);
}

int startswith(const char *string, const char *startwith) {
	int stringlen, startwithlen, i;
	if ((stringlen = strlen(string)) < (startwithlen = strlen(startwith)))
		return 0;
	for (i = 0; i < startwithlen; i++)
		if (string[i] != startwith[i])
			return 0;
	return startwithlen;
}

char* removefirstword(const char *string) {
	char *pos;
	if ((pos = strchr(string, ' ')) != NULL)
		return pos + 1;
	else
		return NULL;
}

char* getrelativepath(const char *dir) {
	char *newpath;
	int len;
	if (dir == NULL)
		return NULL;
	len = strlen(dir);
	newpath = malloc(sizeof(newpath) * PATH_MAX);
	if (len > 0 && dir[0] == '~')
		sprintf(newpath, "%s%s", getenv("HOME"), dir + 1);
	else
		sprintf(newpath, "%s", dir);
	return newpath;
}

void patherrorhandling(const char *path) {
	switch (errno) {
	case EACCES:
		fprintf(stderr, "You don't have the proper permissions for: %s\n", path);
		break;
	case ELOOP:
		fprintf(stderr, "There's a symlink loop in the path: %s\n", path);
		break;
	case ENAMETOOLONG:
		fprintf(stderr,
				"The path length exceeds the c implementations maximum: %s\n",
				path);
		break;
	case ENOENT:
		fprintf(stderr, "The path specified does not name a directory: %s\n",
				path);
		break;
	case ENOTDIR:
		fprintf(stderr, "Parts of the path is not a directory: %s\n", path);
		break;
	default:
		fprintf(stderr, "There's no such directory: %s\n", path);
		break;
	}
}

static char* makestring(int left, int right, const char *string) {
	char *newstr = malloc(sizeof(newstr) * (right - left));
	memcpy(newstr, string + left, right - left);
	newstr[right - left] = '\0';
	return newstr;
}

static int checkvalid(char c) {
	return (c != ' ' && c != '\t' && c != '\n') ? 1 : 0;
}
