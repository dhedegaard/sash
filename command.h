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
void cmd_cd(const char *dir);
/**
 * Attempts to list the contains in a directory, if no argument
 * is supplied (NULL), the current directory is listed.
 */
void cmd_ls(const char *dir);
/**
 * Lists all the environmental variables available on the system.
 */
void cmd_environ();
/**
 * Does a simple echo of the text specified, if the text is NULL,
 * the function returns immediately.
 */
void cmd_echo(const char *str);
/**
 * This happens if all else fails, it simply forks and execs with
 * the command specified.
 */
void _cmd_exec(const char *cmd);
void cmd_exec(arg_t *args);
#endif /* COMMAND_H_ */
