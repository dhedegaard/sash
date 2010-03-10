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
char* makestring(int left, int right, const char *string);
/**
 * Checks to see if a char is "valid", meaning it shouldn't get trimmed.
 */
int checkvalid(char c);

char* trim(const char *string) {
	int left, right = 0, i;
	if (string == NULL)
		return NULL;
	left = 0;
	right = strlen(string) - 1;
	for (i = 0; i <= right; i++)
		if (!checkvalid(string[i]))
			left = i + 1;
		else
			break;
	for (i = right; i >= left; i--)
		if (!checkvalid(string[i]))
			right = i - 1;
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
	for (i = 0;i < startwithlen;i++)
		if (string[i] != startwith[i])
			return 1;
	return 0;
}

char *removefirstword(const char *string) {
	int pos = -1, len, i;
	char *newstring;
	if (string == NULL)
		return NULL;
	if ((len = strlen(string)) == 0)
		return NULL;
	for (i = 0;i < len;i++)
		if (string[i] == ' ') {
			if ((pos = i + 1) > len)
				return NULL;
			break;
		}
	if (pos == -1)
		return NULL;
	newstring = malloc(sizeof(*newstring) * len - pos + 1);
	memcpy(newstring, string + pos, len - pos);
	return newstring;
}

char* makestring(int left, int right, const char *string) {
	int size = right - left + 1;
	char *newstr = malloc(sizeof(newstr) * size + 1);
	memcpy(newstr, string + left, size);
	newstr[size + 1] = '\0';
	return newstr;
}

int checkvalid(char c) {
	return (c != ' ' && c != '\t' && c != '\n') ? 1 : 0;
}
