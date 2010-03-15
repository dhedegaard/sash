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
#include <stdlib.h>
#include <string.h>

/**
 * Does a simple ls to stdout, if dir is NULL,
 * it does ls to the current dir.
 * The implementation is by using a linked list when reading dirs,
 * then putting the strings into an array and freeing the nodes in
 * linked list, calling qsort on the array, printing the strings and
 * the freeing the array again.
 */
void ls_ls(const char *dir);

#endif /* LS_H_ */
