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
 * A simple repressentation of an argument, from a commandline.
 * Should work like a queue in the parser_t struct.
 */
struct arg {
	/**
	 * The next node after this one, if one exists. Should be NULL
	 * if there's no next.
	 */
	struct arg* next;
	/**
	 * The argument contained in the arg struct.
	 */
	char* argv;
};
/**
 * An internal parser type, used for parsing a command, then
 * this struct should be made into an argt_t struct.
 */
struct parser_t {
	/**
	 * The front of the queue.
	 */
	struct arg* front;
	/**
	 * The back of the queue.
	 */
	struct arg* back;
	/**
	 * The size of the queue.
	 */
	int queue_size;
	/**
	 * Is there an ampersand ?
	 */
	int background;
	/**
	 * Is there an '<' ?
	 */
	int inPipe;
	/**
	 * Is there an '>' ?
	 */
	int outPipe;
	/**
	 * If inPipe != 0, where should input be directed.
	 */
	char* inPipeFile;
	/**
	 * If outPipe != null, where should output be directed.
	 */
	char* outPipeFile;
};

/**
 * Remeber to free the parser_t after use, since it's allocated
 * on the heap.
 */
static struct parser_t* createParser_t();

/**
 * Frees a parser_t, including a queue, if there's any, as well
 * as inPipeFile and outPipeFile.
 */
static int destroyParser_t(struct parser_t*);

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
static struct arg_t *parseargs(const char *input);
/**
 * Cleans up after the arg_t struct is ready to be freed
 * from the heap.
 * Returns 0 on success, -1 of error.
 */
static int destroyArg_t(struct arg_t *arg);

void parse(const char *input) {
	struct arg_t *arg = parseargs(input);
	if (arg->cmd == NULL || *(arg->cmd) == '\0') {
		if (arg)
			destroyArg_t(arg);
		return;
	}
	if (strcmp("quit", arg->cmd) == 0 || strcmp("exit", arg->cmd) == 0) {
		if (arg)
			destroyArg_t(arg);
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
	} else if (startswith(arg->cmd, "cd") != 0 && ((strlen(arg->cmd) > 2
			&& arg->cmd[2] == ' ') || strlen(arg->cmd) == 2))
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
		destroyArg_t(arg);
}

static struct arg_t *parseargs(const char *_input) {
	struct arg_t *arg = malloc(sizeof(*arg));
	char *input = trim(_input);
	arg->argv = NULL;
	arg->cmd = NULL;
	arg->inputfile = NULL;
	arg->outputfile = NULL;
	arg->argc = 0;
	arg->background = 0;
	/* parse cmd */
	arg->cmd = malloc(strlen(input) + 1);
	strcpy(arg->cmd, input);
	/* parse argv (without pipes) */
	{
		const char *len = input;
		char *toargs = NULL;
		int last = 0;
		while (*len != '\0' && *len != '<' && *len != '>' && *len != '&')
			len++;
		arg->background = *len == '&';
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

static int destroyArg_t(struct arg_t *arg) {
	if (arg == NULL)
		return -1;
	else {
		char **cp = arg->argv;
		if (cp)
			while (*cp != NULL) {
				free(*cp);
				cp++;
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
	int argcount = 0, i = 0, lastwasspace = 1, len, j = 0, arrlen = 0;
	if (cmd == NULL)
		return NULL;
	pcmd = cmd;
	while (*pcmd != '\0' && *pcmd != '<' && *pcmd != '>' && *pcmd != '&') {
		if (isspace(*pcmd) && !lastwasspace)
			lastwasspace = 1;
		else if (!isspace(*pcmd) && lastwasspace) {
			lastwasspace = 0;
			arrlen++;
		} else if (*pcmd == '\\' && *(pcmd + 1) == ' ' && pcmd != cmd)
			pcmd++;
		pcmd++;
	}
	len = strlen(cmd);
	while (isspace(*(cmd + len - 1)) && *(cmd + len - 2) != '\\')
		len--;
	if (len == 0) {
		args = malloc(sizeof(*args));
		*args = malloc(2);
		**args = '\0';
		*(args + 1) = NULL;
		return args;
	}
	args = calloc(sizeof(*args), arrlen + 2);
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

static struct parser_t* createParser_t() {
	struct parser_t* p = malloc(sizeof(*p));
	/* null pointers. */
	p->back = NULL;
	p->front = NULL;
	p->inPipeFile = NULL;
	p->outPipeFile = NULL;
	/* integer initialization */
	p->background = 0;
	p->inPipe = 0;
	p->outPipe = 0;
	p->queue_size = 0;
	return p;
}

static int destroyParser_t(struct parser_t* p) {
	if (!p)
		return 0;
	if (p->inPipeFile)
		free(p->inPipeFile);
	if (p->outPipeFile)
		free(p->outPipeFile);
	while (p->front) {
		struct arg* a = p->front;
		p->front = p->front->next;
		free(a);
	}
	return 1;
}
