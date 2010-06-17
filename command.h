/*
 * command.h
 *
 *  Created on: 08/03/2010
 *      Author: Dennis Hedegaard
 */

#ifndef COMMAND_H_
#define COMMAND_H_

#include "parser.h"

/**
 * Prints some help, this might include possible commands.
 */
void cmd_help();
/**
 * Quits the shell.
 */
void cmd_quit();
/**
 * Attempts to change directory, to the one specified. If
 * the directory does not exist, or is restricted access,
 * the error is handled internally.
 * If the change is succesfully, the PWD variable is changed.
 */
void cmd_cd(const char*);
/**
 * Attempts to list the contains in a directory, if no argument
 * is supplied (NULL), the current directory is listed.
 */
void cmd_ls(const char*);
/**
 * Lists all the environmental variables available on the system.
 */
void cmd_environ();
/**
 * Does a simple echo of the text specified, if the text is NULL,
 * the function returns immediately.
 */
void cmd_echo(const char*);
/**
 * This happens if all else fails, it simply forks and execs with
 * the command specified.
 */
void cmd_exec(struct arg_t*);
/**
 * Clears the screen and puts the cursor at the top left corner.
 * The implementation is most likely the clear screen and move cursor
 * to top left corner ANSI sequence.
 */
void cmd_clear();
#endif /* COMMAND_H_ */
