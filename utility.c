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
#include <ctype.h>

#include "utility.h"

/* tested with:
 int main() {
 assert(trim(NULL) == NULL);
 assert(strcmp(trim("  "), "") == 0);
 assert(strcmp(trim("hej"), "hej") == 0);
 assert(strcmp(trim(" hej"), "hej") == 0);
 assert(strcmp(trim("hej "), "hej") == 0);
 assert(strcmp(trim(" hej "), "hej") == 0);
 assert(strcmp(trim("   eqwqwe  eqw     "), "eqwqwe  eqw") == 0);
 assert(strcmp(trim("  hej  "), "hej") == 0);
 assert(strcmp(trim(" hej  "), "hej") == 0);
 return 0;
 */
char* trim(const char *str) {
	char *result = NULL;
	const char *strp;
	int len;
	if (str == NULL)
		return NULL;
	while (isspace(*str))
		str++;
	if (*str == '\0') {
		result = malloc(1);
		*result = '\0';
		return result;
	}
	strp = str;
	while (*strp != '\0')
		strp++;
	if (*strp == '\0')
		strp--;
	while (isspace(*strp) || *strp == '\n')
		strp--;
	len = strp - str + 1;
	result = calloc(sizeof(*result), len);
	memcpy(result, str, len);
	*(result + len) = '\0';
	return result;
}

/**
 * Tested with:
 int main() {
 assert(startswith(NULL, NULL) == 0);
 assert(startswith("hej", NULL) == 0);
 assert(startswith(NULL, "hej") == 0);
 assert(startswith("hejsa", "hej") == 1);
 assert(startswith("davs hej", "davs ") == 1);
 assert(startswith("dav", "dav") == 1);
 assert(startswith("dav", "davs") == 0);
 return 0;
 }
 */
int startswith(const char *string, const char *startwith) {
	if (string == NULL || startwith == NULL)
		return 0;
	while (*string == *startwith) {
		if (*string == '\0')
			return 1;
		string++;
		startwith++;
	}
	if (*startwith == '\0')
		return 1;
	else
		return 0;
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
