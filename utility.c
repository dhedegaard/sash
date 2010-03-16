/*
 * utility.c
 *
 *  Created on: 08/03/2010
 *      Author: Dennis Hedegaard
 */

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
		return 1;
	for (i = 0; i < startwithlen; i++)
		if (string[i] != startwith[i])
			return 1;
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
	newpath = malloc(sizeof(newpath) * MAX_LINE_LENGTH);
	if (len > 0 && dir[0] == '~')
		sprintf(newpath, "%s%s", getenv("HOME"), dir + 1);
	else
		sprintf(newpath, "%s", dir);
	return newpath;
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
