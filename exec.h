/*
 * exec.h
 *
 *  Created on: 17/03/2010
 *      Author: Dennis Hedegaard
 */

#ifndef EXEC_H_
#define EXEC_H_

#include "parser.h"

/**
 * Parses and executes a command accordingly.
 */
/* int exec2(const char *cmd); */
int exec(arg_t*);

#endif /* EXEC_H_ */
