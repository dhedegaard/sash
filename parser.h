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
	char *cmd;
	char *inputfile;
	char *outputfile;
	int argv_count;
	char **argv;
} arg_t;

/**
 * Parses a string, anything is called from this method
 * and into the command part of the code.
 */
void parse(const char *input);
/**
 * Parse a command to a arg_t struct, this struct makes
 * it easy to use in command implementation.
 * Everything's allocated on the heap.
 * If an error occured, NULL is returned.
 */
arg_t *parseargs(const char *input);
/**
 * Cleans up after the arg_t struct is ready to be freed
 * from the heap.
 * Returns 0 on success, -1 of error.
 */
int closeargs(arg_t *arg);

#endif /* PARSER_H_ */
