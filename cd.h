/*
 * cd.h
 *
 *  Created on: 16/03/2010
 *      Author: Dennis Hedegaard
 */

#ifndef CD_H_
#define CD_H_

/**
 * Attempts to change to a particular directory, if that
 * directory doesn't exist, permissions are denied, the
 * maximum path length is exceeded then it's handled
 * internally.
 */
void cd_cd(const char*);

#endif /* CD_H_ */
