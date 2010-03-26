/*
 * exec.c
 *
 *  Created on: 17/03/2010
 *      Author: Dennis Hedegaard
 */

#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/wait.h>
#include <stdio.h>

#include "exec.h"

/**
 * Execute and return both input and output to different file.
 */
static int execiopipe(const char *cmd, const char *inputfile,
		const char *outputfile);
/**
 * Attempts to parse the next "word" (file) from the offset specified,
 * if no word's after the offset before the line is ending, null is returned.
 * It passes by all ' ' before it encounters anything real, when it does
 * it no longer passes ' ' but returns the string after.
 * Remember to free the pointer, after usage.
 */
static char* parsenextword(const char *cmd, int offset);
/**
 * Takes out the command part of a line, that is the first "word".
 * Remember to free the pointer.
 */
static char* parsecmd(const char *cmd);
/**
 * Parses a cmd to a args string for execvp
 */
static char** parsetoargs(const char *cmd);

int exec(const char *cmd) {
	char *inputfile = NULL, *outputfile = NULL, *realcmd = NULL;
	int i, len = strlen(cmd), lineend = -1;
	for (i = 0; i < len; i++)
		switch (cmd[i]) {
		case '<':
			inputfile = parsenextword(cmd, i + 1);
			if (lineend == -1)
				lineend = i;
			break;
		case '>':
			outputfile = parsenextword(cmd, i + 1);
			if (lineend == -1)
				lineend = i;
			break;
		}
	if (lineend != -1) {
		int i;
		realcmd = malloc(strlen(cmd) - lineend + 2);
		for (i = 0; i < lineend; i++)
			if ((cmd[i] == '<' || cmd[i] == '>') && i > 0)
				realcmd[i] = '\0';
			else
				realcmd[i] = cmd[i];
		if (realcmd[i - 1] != '\0')
			realcmd[i] = '\0';
	} else
		realcmd = (char*) cmd;
	execiopipe(realcmd, inputfile, outputfile);
	if (inputfile)
		free(inputfile);
	if (outputfile)
		free(outputfile);
	return 0;
}

static int execiopipe(const char *cmd, const char *inputfile,
		const char *outputfile) {
	int res = 1000, childpid;
	FILE *fin, *fout;
	char *execcmd, **cmdargs;
	switch (childpid = fork()) {
	case -1:
		strerror(errno);
		return errno;
		break;
	case 0:
		if (inputfile != NULL) {
			if ((fin = freopen(inputfile, "r", stdin)) == NULL) {
				strerror(errno);
				_exit(errno);
			}
		}
		if (outputfile != NULL) {
			if ((fout = freopen(outputfile, "a", stdout)) == NULL) {
				strerror(errno);
				_exit(errno);
			}
		}
		execcmd = parsecmd(cmd);
		cmdargs = parsetoargs(cmd);
		res = execvp(execcmd, cmdargs);
		if (res == -1) {
			char *err;
			if ((err = strerror(errno)) == NULL)
				fprintf(stderr, "%s: unknown error, errno: %d.\n", execcmd,
						errno);
			else
				fprintf(stderr, "%s: %s\n", execcmd, err);
		}
		if (execcmd)
			free(execcmd);
		if (cmdargs)
			free(cmdargs);
		if (fin)
			fclose(fin);
		if (fout)
			fclose(fout);
		exit(res);
		break;
	default:
		waitpid(childpid, &res, 0);
		break;
	}
	return res;
}

/**
 * tests run:
 int main() {
 assert(parsenextword(NULL, 0) == NULL);
 assert(parsenextword(NULL, -10) == NULL);
 assert(parsenextword(NULL, 100) == NULL);
 assert(strcmp(parsenextword("hej dav hej2", 0), "hej") == 0);
 assert(strcmp(parsenextword("hej dav hej2", 3), "dav") == 0);
 assert(strcmp(parsenextword("hej dav hej2", 7), "hej2") == 0);
 assert(parsenextword("hej dav hej2", -10) == 0);
 assert(strcmp(parsenextword(" hej dav hej2", 0), "hej") == 0);
 assert(strcmp(parsenextword(" hej dav hej2", 4), "dav") == 0);
 assert(strcmp(parsenextword(" hej dav hej2", 9), "hej2") == 0);
 assert(parsenextword(" hej dav hej2", -10) == 0);
 assert(parsenextword(" hej dav hej2", 15) == 0);
 assert(parsenextword(" hej dav hej2", 15) == 0);
 assert(strcmp(parsenextword(" hej  dav  hej2  ", 0), "hej") == 0);
 assert(strcmp(parsenextword(" hej  dav  hej2  ", 5), "dav") == 0);
 assert(strcmp(parsenextword(" hej  dav  hej2  ", 9), "hej2") == 0);
 assert(parsenextword("hej         ", 6) == NULL);
 printf("test complete.\n");
 return(0);
 }
 */
static char* parsenextword(const char *cmd, int offset) {
	int len, i, pos = 0;
	char *result;
	if (cmd == NULL)
		return NULL;
	while (isspace(*cmd)) {
		cmd++;
		offset -= offset > 0 ? 1 : 0;
	}
	len = strlen(cmd);
	if (offset < 0 || offset >= len)
		return NULL;
	result = malloc(len - offset + 1);
	for (i = offset; i < len; i++)
		if (cmd[i] == ' ' && pos == 0)
			;
		else if (cmd[i] != ' ')
			result[pos++] = cmd[i];
		else if (cmd[i] == ' ' && pos != 0)
			break;
	result[pos] = '\0';
	if (*result == '\0') {
		free(result);
		return NULL;
	} else
		return result;
}

/**
 * Tested with:
 int main() {
 assert(parsecmd(NULL) == NULL);
 assert(strcmp(parsecmd(""), "") == 0);
 assert(strcmp(parsecmd("  "), "") == 0);
 assert(strcmp(parsecmd("hej"), "hej") == 0);
 assert(strcmp(parsecmd("hej "), "hej") == 0);
 assert(strcmp(parsecmd("hej dav"), "hej") == 0);
 assert(strcmp(parsecmd(" hej"), "hej") == 0);
 assert(strcmp(parsecmd(" hej "), "hej") == 0);
 assert(strcmp(parsecmd("   hej dav     "), "hej") == 0);
 printf("test complete!\n");
 return 0;
 }
 */
static char* parsecmd(const char *cmd) {
	int len = 0;
	char *result;
	const char *pcmd;
	if (cmd == NULL)
		return NULL;
	/* move past starting spaces, or if the string
	 * is empty, to the null terminator.
	 */
	while (isspace(*cmd) && *cmd != '\0')
		cmd++;
	/* If the string is empty or all spaces, return
	 * an empty string.
	 */
	if (*cmd == '\0') {
		result = malloc(1);
		*result = '\0';
		return result;
	}
	pcmd = cmd;
	/* from the start of the command, run len++ until
	 * a space or null terminator is found.
	 */
	while (!isspace(*pcmd) && *pcmd != '\0') {
		pcmd++;
		len++;
	}
	/* Now allocate len + 1 bytes, copy len bytes from
	 * cmd to result, null terminate the string and return.
	 */
	result = malloc(len + 1);
	pcmd = cmd;
	memcpy(result, cmd, len);
	*(result + len) = '\0';
	return result;
}

/**
 * Tests run:
 int main() {
 assert(parsetoargs(NULL) == NULL);
 assert(strcmp(parsetoargs("")[0], "") == 0);
 assert(strcmp(parsetoargs("  ")[0], "") == 0);
 assert(strcmp(parsetoargs("hej")[0], "hej") == 0);
 assert(parsetoargs("hej")[1] == NULL);
 assert(strcmp(parsetoargs(" hej")[0], "hej") == 0);
 assert(strcmp(parsetoargs("hej ")[0], "hej") == 0);
 assert(strcmp(parsetoargs("   hej   ")[0], "hej") == 0);
 assert(strcmp(parsetoargs("hej dav")[0], "hej") == 0);
 assert(strcmp(parsetoargs("hej dav")[1], "dav") == 0);
 assert(strcmp(parsetoargs("  hej dav")[0], "hej") == 0);
 assert(strcmp(parsetoargs("  hej dav")[1], "dav") == 0);
 assert(strcmp(parsetoargs("  hej  dav")[0], "hej") == 0);
 assert(strcmp(parsetoargs("  hej  dav")[1], "dav") == 0);
 assert(strcmp(parsetoargs("hej dav  ")[0], "hej") == 0);
 assert(strcmp(parsetoargs("hej dav  ")[1], "dav") == 0);
 assert(strcmp(parsetoargs("hej  dav   ")[0], "hej") == 0);
 assert(strcmp(parsetoargs("hej  dav   ")[1], "dav") == 0);
 assert(strcmp(parsetoargs("  hej dav  ")[0], "hej") == 0);
 assert(strcmp(parsetoargs("  hej dav  ")[1], "dav") == 0);
 assert(strcmp(parsetoargs("   hej  dav   ")[0], "hej") == 0);
 assert(strcmp(parsetoargs("   hej  dav   ")[1], "dav") == 0);
 assert(strcmp(parsetoargs("hej dav svend")[0], "hej") == 0);
 assert(strcmp(parsetoargs("hej dav svend")[1], "dav") == 0);
 assert(strcmp(parsetoargs("hej dav svend")[2], "svend") == 0);
 assert(parsetoargs("hej dav svend")[3] == NULL);
 printf("test completed.\n");
 return 0;
 }
 */
static char** parsetoargs(const char *cmd) {
	char **args;
	const char* pcmd;
	int argcount = 0, i, lastwasspace = 1, len, j = 0, arrlen = 1;
	if (cmd == NULL)
		return NULL;
	pcmd = cmd;
	while (*pcmd != '\0') {
		if (isspace(*pcmd) && !lastwasspace)
			lastwasspace = 1;
		else if (!isspace(*pcmd) && lastwasspace) {
			lastwasspace = 0;
			arrlen++;
		} else if (*cmd == '\\' && *(cmd + 1) == ' ' && pcmd != cmd)
			pcmd++;
		pcmd++;
	}
	args = malloc(sizeof(*args) * arrlen);
	len = strlen(cmd);
	while (isspace(*(cmd + len - 1)) && *(cmd + len - 2) != '\\')
		len--;
	if (len == 0) {
		args[0] = malloc(1);
		args[0][0] = '\0';
		args[1] = NULL;
		return args;
	}
	for (i = 0; i < arrlen; i++)
		args[i] = NULL;
	lastwasspace = 1;
	for (i = 0; i < len; i++)
		if (lastwasspace && cmd[i] != ' ') {
			lastwasspace = 0;
			j = 0;
			args[argcount] = malloc(len + 1);
			args[argcount][j++] = cmd[i];
		} else if (!lastwasspace && cmd[i] == ' ') {
			lastwasspace = 1;
			args[argcount++][j] = '\0';
		} else if (!lastwasspace && cmd[i] != ' ')
			args[argcount][j++] = cmd[i];
		else if (cmd[i] == '\\' && cmd[i + 1] != ' ') {
			args[argcount][j++] = ' ';
			i++;
		}
	args[argcount][j] = '\0';
	return args;
}
