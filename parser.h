/*
 * parser.h
 *
 *  Created on: 08/03/2010
 *      Author: Dennis Hedegaard
 */

#ifndef PARSER_H_
#define PARSER_H_

/**
 * A simple struct, that holds the initial cmd, an optional
 * inputfile, an optional outputfile, an array of args for
 * exec'ing etc, and a count of these.
 */
typedef struct {
	/* the full cmd. */
	char *cmd;
	/* inputfile parsed from cmd. */
	char *inputfile;
	/* outputfile parsed from cmd. */
	char *outputfile;
	/* count of args, without pipes. */
	int argc;
	/* args, without pipes, NULL terminated. */
	char **argv;
} arg_t;

/**
 * Parses a string, anything is called from this method
 * and into the command part of the code.
 */
void parse(const char*);

#endif /* PARSER_H_ */
