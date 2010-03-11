/*
 * parser.h
 *
 *  Created on: 08/03/2010
 *      Author: Dennis Hedegaard
 */

#ifndef PARSER_H_
#define PARSER_H_

#include <string.h>

#include "global.h"
#include "utility.h"
#include "command.h"

/**
 * Parses a string, anything is called from this method
 * and into the command part of the code.
 */
void parse(const char *input);

#endif /* PARSER_H_ */
