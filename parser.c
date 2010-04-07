/*
 * parser.c
 *
 *  Created on: 08/03/2010
 *      Author: Dennis Hedegaard
 */

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "utility.h"
#include "command.h"
#include "parser.h"

/**
 * Parses a cmd to args,
 */
static char** parsetoargs(const char *cmd);
/**
 * Parse both input and output pipes, the pipe is decided upon
 * the pipechar supplied, '<' for input, and '>' for output.
 * If an error occured or cmd is NULL, NULL is returned.
 */
static char *parsepipefromcmd(const char *cmd, char pipechar);
/**
 * Parse a command to a arg_t struct, this struct makes
 * it easy to use in command implementation.
 * Everything's allocated on the heap.
 * If an error occured, NULL is returned.
 */
static arg_t *parseargs(const char *input);
/**
 * Cleans up after the arg_t struct is ready to be freed
 * from the heap.
 * Returns 0 on success, -1 of error.
 */
static int closeargs(arg_t *arg);

void parse(const char *input) {
	arg_t *arg = parseargs(input);
	if (arg->cmd == NULL || *(arg->cmd) == '\0') {
		if (arg)
			closeargs(arg);
		return;
	}
	if (strcmp("quit", arg->cmd) == 0 || strcmp("exit", arg->cmd) == 0) {
		if (arg)
			closeargs(arg);
		cmd_quit();
	} else if (strcmp("help", arg->cmd) == 0)
		cmd_help();
	else if (startswith(arg->cmd, "echo ") != 0) {
		char *cmd = arg->cmd;
		while (!isspace(*cmd))
			cmd++;
		while (isspace(*cmd))
			cmd++;
		cmd_echo(cmd);
	} else if (strcmp("environ", arg->cmd) == 0)
		cmd_environ();
	else if (startswith(arg->cmd, "ls") != 0 || startswith(arg->cmd, "dir")
			!= 0) {
		char *cmd = arg->cmd;
		while (!isspace(*cmd) && *cmd != '\0')
			cmd++;
		while (isspace(*cmd) && *cmd != '\0')
			cmd++;
		if (*cmd == '\0')
			cmd_ls(NULL);
		else
			cmd_ls(cmd);
	} else if (startswith(arg->cmd, "cd") != 0)
		if (strchr(arg->cmd, ' ') != NULL) {
			char *cmd = arg->cmd;
			while (!isspace(*cmd) && *cmd != '\0')
				cmd++;
			while (isspace(*cmd) && *cmd != '\0')
				cmd++;
			cmd_cd(cmd);
		} else
			cmd_cd(NULL);
	else if (startswith(arg->cmd, "clr") != 0 || startswith(arg->cmd, "clear")
			!= 0)
		cmd_clear();
	else if (*arg->cmd != '\0')
		cmd_exec(arg);
	if (arg)
		closeargs(arg);
}

static arg_t *parseargs(const char *_input) {
	arg_t *arg = malloc(sizeof(*arg));
	char *input = trim(_input);
	arg->argv = NULL;
	arg->cmd = NULL;
	arg->inputfile = NULL;
	arg->outputfile = NULL;
	/* parse cmd */
	{
		int len = strlen(input);
		char *cmd = malloc(len + 1);
		memcpy(cmd, input, len);
		cmd[len] = '\0';
		arg->cmd = cmd;
	}
	/* parse argv (without pipes) */
	{
		const char *len = input;
		char *toargs = NULL;
		int last = 0;
		while (1) {
			char c = *len;
			if (c == '\0' || c == '<' || c == '>')
				break;
			else
				len++;
		}
		last = len - input;
		toargs = malloc(last + 1);
		memcpy(toargs, input, last);
		toargs[last] = '\0';
		arg->argv = parsetoargs(toargs);
		free(toargs);
	}
	/* parse argv_count */
	{
		int count = 0;
		char **argcount = arg->argv;
		while (*(argcount++) != NULL)
			count++;
		/* for (argcount = arg->argv; *argcount != NULL; argcount++)
		 ; */
		arg->argc = count;
	}
	/* parse inputfile */
	arg->inputfile = parsepipefromcmd(input, '<');
	/* parse outputfile */
	arg->outputfile = parsepipefromcmd(input, '>');
	if (input)
		free(input);
	return arg;
}

static int closeargs(arg_t *arg) {
	if (arg == NULL)
		return -1;
	else {
		/* char **cp = arg->argv;*/
		if (/* cp */arg->argv) {
			int c = arg->argc, i = 0;
			for (; i < c; i++)
				free(arg->argv[i]);
			/*while (*cp != NULL) {
			 free(*cp);
			 cp++;
			 }*/
		}
		if (arg->argv != NULL)
			free(arg->argv);
		if (arg->cmd != NULL)
			free(arg->cmd);
		if (arg->inputfile)
			free(arg->inputfile);
		if (arg->outputfile)
			free(arg->outputfile);
		free(arg);
	}
	return 0;
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
	char **args = NULL;
	const char* pcmd = NULL;
	int argcount = 0, i = 0, lastwasspace = 1, len, j = 0, arrlen = 1;
	if (cmd == NULL)
		return NULL;
	pcmd = cmd;
	while (*pcmd != '\0' && *pcmd != '<' && *pcmd == '>') {
		if (isspace(*pcmd) && !lastwasspace)
			lastwasspace = 1;
		else if (!isspace(*pcmd) && lastwasspace) {
			lastwasspace = 0;
			arrlen++;
		} else if (*cmd == '\\' && *(cmd + 1) == ' ' && pcmd != cmd)
			pcmd++;
		pcmd++;
	}
	len = strlen(cmd);
	while (isspace(*(cmd + len - 1)) && *(cmd + len - 2) != '\\')
		len--;
	if (len == 0) {
		args = malloc(sizeof(*args));
		args[0] = malloc(1);
		args[0][0] = '\0';
		args[1] = NULL;
		return args;
	}
	args = calloc(sizeof(*args), arrlen + 1);
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
	args[++argcount] = NULL;
	return args;
}

static char *parsepipefromcmd(const char *cmd, char pipechar) {
	char *pipe = NULL;
	while (*cmd != '\0') {
		if (*cmd == pipechar) {
			int len = 0;
			cmd++;
			while (isspace(*cmd) && *cmd != '\0')
				cmd++;
			if (*cmd == '\0')
				return NULL;
			while (!isspace(*(cmd + len)) && *(cmd + len) != '\0' && *(cmd
					+ len) != '<' && *(cmd + len) != '>')
				len++;
			if (len == 0)
				return NULL;
			pipe = malloc(len + 1);
			memcpy(pipe, cmd, len + 1);
			pipe[len] = '\0';
		}
		cmd++;
	}
	return pipe;
}
