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
 assert(strcmp(trim("hej\n"), "hej") == 0);
 assert(strcmp(trim("  hej  \n"), "hej") == 0);
 assert(strcmp(trim("  hej sa  \n  \n"), "hej sa") == 0);
 assert(strcmp(trim("  \n   hej"), "hej") == 0);
 assert(strcmp(trim("\n    \nhej\n    \n"), "hej") == 0);
 assert(strcmp(trim("hej\nhej\n"), "hej\nhej") == 0);
 return 0;
 }
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
	while (isspace(*strp))
		strp--;
	len = strp - str + 1;
	result = calloc(sizeof(*result), len + 1);
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
	char *newpath = NULL;
	int len = 0;
	if (dir == NULL)
		return NULL;
	len = strlen(dir);
	if (len > 0 && dir[0] == '~') {
		char *home = getenv("HOME");
		int homelen = strlen(home);
		newpath = malloc(len + homelen + 1);
		snprintf(newpath, homelen + len, "%s%s", home, dir + 1);
		*(newpath + homelen + len) = '\0';
	} else {
		newpath = malloc(len + 1);
		snprintf(newpath, len + 1, "%s", dir);
		*(newpath + len) = '\0';
	}
	return newpath;
}
