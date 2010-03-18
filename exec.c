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

#include "exec.h"

/**
 * Execute without redirection.
 */
static int execnopipe(const char *cmd);
/**
 * Execute and redirect input to an file.
 */
static int execipipe(const char *cmd, const char *inputfile);
/**
 * Execute and redirect output to a file.
 */
static int execopipe(const char *cmd, const char *outputfile);
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
 * Takes out the arguments for a command, this is everything after
 * a command, before redirects (if any).
 * Remember to free the pointer.
 */
/* static char* parseargs(const char *cmd);*/
/**
 * Parses a cmd to a args string for execvp
 */
static char** parsetoargs(const char *cmd);
/**
 * Parse the command line without pipes.
 */
static char* parsewithoutpipes(const char* cmd);

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
	if (inputfile == NULL) {
		if (outputfile == NULL)
			execnopipe(realcmd);
		else
			execopipe(realcmd, outputfile);
	} else {
		if (outputfile == NULL)
			execipipe(realcmd, inputfile);
		else
			execiopipe(realcmd, inputfile, outputfile);
	}
	if (inputfile)
		free(inputfile);
	if (outputfile)
		free(outputfile);
	return 0;
}

static int execnopipe(const char *cmd) {
	int res = 100, childpid;
	switch ((childpid = fork())) {
	case -1:
		strerror(errno);
		return errno;
		break;
	case 0:
		res = execvp(parsecmd(cmd), parsetoargs(cmd));
		exit(res);
		break;
	}
	return res;
}

static int execipipe(const char *cmd, const char *inputfile) {
	int res = 100, fin;
	switch (fork()) {
	case -1:
		strerror(errno);
		return errno;
		break;
	case 0:
		if ((fin = open(inputfile, O_RDONLY)) == -1) {
			strerror(errno);
			res = errno;
			_exit(errno);
		}
		close(0);
		dup(fin);
		res = execvp(parsecmd(cmd), parsetoargs(cmd));
		close(fin);
		exit(res);
		break;
	}
	return res;
}
static int execopipe(const char *cmd, const char *outputfile) {
	int res = 100, fout;
	switch (fork()) {
	case -1:
		strerror(errno);
		return errno;
		break;
	case 0:
		if ((fout = open(outputfile, O_WRONLY + O_CREAT, 0755)) == -1) {
			strerror(errno);
			res = errno;
			_exit(errno);
		}
		close(1);
		dup(fout);
		res = execvp(parsecmd(cmd), parsetoargs(cmd));
		close(fout);
		exit(res);
		break;
	}
	return res;
}
static int execiopipe(const char *cmd, const char *inputfile,
		const char *outputfile) {
	int res = 1000, fin, fout;
	switch (fork()) {
	case -1:
		strerror(errno);
		return errno;
		break;
	case 0:
		if ((fin = open(inputfile, O_RDONLY)) == -1) {
			strerror(errno);
			res = errno;
			_exit(errno);
		}
		if ((fout = open(outputfile, O_WRONLY + O_CREAT, 0755)) == -1) {
			strerror(errno);
			res = errno;
			_exit(errno);
		}
		close(0);
		dup(fin);
		close(1);
		dup(fout);
		res = execvp(parsecmd(cmd), parsetoargs(cmd));
		close(fin);
		close(fout);
		exit(res);
		break;
	}
	return res;
}

static char* parsenextword(const char *cmd, int offset) {
	int len = strlen(cmd), i, pos = 0;
	char *result;
	if (offset >= len)
		return NULL;
	result = malloc(sizeof(*result) * (len - offset + 1));
	for (i = offset; i < len; i++)
		if (cmd[i] == ' ' && pos == 0)
			;
		else if (cmd[i] != ' ')
			result[pos++] = cmd[i];
		else if (cmd[i] == ' ' && pos != 0)
			break;
	result[pos] = '\0';
	return result;
}

static char* parsecmd(const char *cmd) {
	int i, len = strlen(cmd);
	char *result;
	if (cmd == NULL)
		return NULL;
	result = malloc(sizeof(*result) * (len + 1));
	for (i = 0; i < len + 1; i++)
		if (cmd[i] == ' ' || cmd[i] == '\0') {
			result[i] = '\0';
			break;
		} else
			result[i] = cmd[i];
	return result;
}

static char** parsetoargs(const char *cmd) {
	char **args = malloc(sizeof(*args) * 10);
	int argcount = 0, i, lastwasspace = 1, len = strlen(cmd), j = 0;
	for (i = 0; i < 10; i++)
		args[i] = NULL;
	for (i = 0; i < len; i++)
		if (lastwasspace && cmd[i] != ' ') {
			lastwasspace = 0;
			j = 0;
			args[argcount] = malloc(len);
			args[argcount][j++] = cmd[i];
		} else if (!lastwasspace && cmd[i] == ' ') {
			lastwasspace = 1;
			args[argcount++][j] = '\0';
		} else if (!lastwasspace && cmd[i] != ' ')
			args[argcount][j++] = cmd[i];
	args[argcount][j] = '\0';
	return args;
}

static char* parsewithoutpipes(const char* cmd) {
	int i, pos = -1, len = strlen(cmd);
	char *result = NULL, *trimmed = NULL;
	result = malloc(len + 1);
	for (i = 0;i < len;i++)
		if (cmd[i] == '<' || cmd[i] == '>') {
			pos = i - 1;
			break;
		}
	if (pos == -1)
		memcpy(result, cmd, len + 2);
	else
		memcpy(result, cmd, pos + 1);
	trimmed = trim(result);
	free(result);
	return trimmed;
}
