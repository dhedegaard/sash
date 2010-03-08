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

char* makestring(int left, int right, const char *string) {
	int size = right - left + 1;
	char *newstr = malloc(sizeof(newstr) * size + 1);
	memset(newstr, '\0', sizeof(newstr) * size + 1);
	memcpy(newstr, string + left, size);
	newstr[size + 1] = '\0';
	return newstr;
}

int checkvalid(char c) {
	return (c != ' ' && c != '\t' && c != '\n') ? 1 : 0;
}
