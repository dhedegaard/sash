/*
 * environ.c
 *
 *  Created on: 10/03/2010
 *      Author: Dennis Hedegaard
 */

#ifndef ENVIRON_H_
#define ENVIRON_H_

/**
 * Sets the env variable, this is used later for "querying".
 * This function saves the length of the parameter as well
 * as holding a pointer to it in a file global scope.
 */
void setenviron(char **env);
/**
 * Takes all environmental values and puts the through the
 * stream with a function like fprintf.
 * If setenviron() hasn't been run yet, this method prints a
 * warning with the function about needing to do so.
 */
void getenviron(FILE* stream, int(*f)(FILE*, const char*, ...));
/**
 * Takes the working dir as stored from the time where setenviron()
 * was run, this is nice for keeping the initial workdir in order.
 * Used for the "help" command, in order to find the readme after using
 * chdir.
 */
const char *getstartwd();
/**
 * Make sure to run this, for some heap cleanups.
 */
void environ_cleanup();

#endif /* ENVIRON_H_ */
