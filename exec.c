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
#include "global.h"

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
static char* parseargs(const char *cmd);

int exec(const char *cmd) {
	char *inputfile = NULL, *outputfile = NULL;
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
	if (inputfile == NULL) {
		if (outputfile == NULL)
			execnopipe(cmd);
		else
			execopipe(cmd, outputfile);
	} else {
		if (outputfile == NULL)
			execipipe(cmd, inputfile);
		else
			execiopipe(cmd, inputfile, outputfile);
	}
	if (inputfile)
		free(inputfile);
	if (outputfile)
		free(outputfile);
	return 0;
}

static int execnopipe(const char *cmd) {
	int res = 100;
	char *_cmd = NULL, *_args = NULL;
	switch (fork()) {
	case -1:
		strerror(errno);
		return errno;
		break;
	case 0:
		_cmd = parsecmd(cmd);
		_args = parseargs(cmd);
		res = execlp(_cmd, _args, NULL);
		_exit(res);
		break;
	}
	return res;
}

static int execipipe(const char *cmd, const char *inputfile) {
	int res = 100, fin;
	char *_cmd, *_args;
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
		_cmd = parsecmd(cmd);
		_args = parseargs(cmd);
		res = execlp(cmd, cmd, NULL);
		close(fin);
		if (_cmd)
			free(_cmd);
		if (_args)
			free(_args);
		_exit(res);
		break;
	}
	return res;
}
static int execopipe(const char *cmd, const char *outputfile) {
	int res = 100, fout;
	char *_cmd = NULL, *_args = NULL;
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
		_cmd = parsecmd(cmd);
		_args = parseargs(cmd);
		res = execlp(cmd, cmd, NULL);
		close(fout);
		if (_cmd)
			free(_cmd);
		if (_args)
			free(_args);
		_exit(res);
		break;
	}
	return res;
}
static int execiopipe(const char *cmd, const char *inputfile,
		const char *outputfile) {
	int res = 1000, fin, fout;
	char *_cmd = NULL, *_args = NULL;
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
		_cmd = parsecmd(cmd);
		_args = parseargs(cmd);
		res = execlp(cmd, cmd, NULL);
		close(fin);
		close(fout);
		if (_cmd)
			free(_cmd);
		if (_args)
			free(_args);
		_exit(res);
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
	return result;
}

static char* parsecmd(const char *cmd) {
	int i, len = strlen(cmd);
	char *result;
	if (cmd == NULL)
		return NULL;
	result = malloc(sizeof(*result) * (len + 1));
	for (i = 0; i < len; i++)
		if (i == ' ') {
			result[i] = '\0';
			break;
		} else
			result[i] = cmd[i];
	return result;
}

static char* parseargs(const char *cmd) {
	int off = -1, i, len = strlen(cmd), pos = 0, endpos = -1;
	char *result;
	malloc(sizeof(*result) * (len + 1));
	for (i = 0; i < len; i++)
		if (cmd[i] == '<' || cmd[i] == '>')
			endpos = i + 1;
	if (endpos == -1)
		endpos = len;
	for (i = 0; i < len; i++)
		if (cmd[i] == ' ' && off == -1)
			continue;
		else if (cmd[i] != ' ')
			off = i;
		else if (cmd[i] == ' ' && off > 0)
			break;
	for (i = off; i < endpos; i++)
		if (cmd[i] == ' ' && pos == 0)
			continue;
		else if (cmd[i] != ' ' && pos == 0)
			pos = i;
		else if (pos != 0)
			result[pos++] = cmd[i];
	return result;
}

