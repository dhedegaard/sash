/*
 * command.h
 *
 *  Created on: 08/03/2010
 *      Author: Dennis Hedegaard
 */

#ifndef COMMAND_H_
#define COMMAND_H_

#include <stdio.h>
#include <stdlib.h>

/**
 * Prints some help, this might include possible commands.
 */
void help();
/**
 * Quits the shell.
 */
void quit();
/**
 * Attempts to change directory, to the one specified. If
 * the directory does not exist, or is restricted access,
 * the error is handled internally.
 * If the change is succesfully, the PWD variable is changed.
 */
void cd(const char *dir);
/**
 * Attempts to list the contains in a directory, if no argument
 * is supplied (NULL), the current directory is listed.
 */
void ls(const char *dir);
/**
 * Lists all the environmental variables available on the system.
 */
void environ();
/**
 * Does a simple echo of the text specified, if the text is NULL,
 * the function returns immediately.
 */
void echo(const char *str);

#endif /* COMMAND_H_ */
