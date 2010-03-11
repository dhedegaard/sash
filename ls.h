/*
 * ls.h
 *
 *  Created on: 11/03/2010
 *      Author: Dennis Hedegaard
 */

#ifndef LS_H_
#define LS_H_

#include <stdio.h>
#include <dirent.h>

/**
 * Does a simple ls to stdout, if dir is NULL,
 * it does ls to the current dir.
 */
void ls_ls(const char *dir);

#endif /* LS_H_ */
