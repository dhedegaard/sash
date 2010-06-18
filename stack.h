/*
 * stack.h
 *
 *  Created on: 16/03/2010
 *      Author: Dennis Hedegaard
 */

#ifndef STACK_H_
#define STACK_H_

/**
 * A stack definition. Used for allowing more stacks operating
 * at the same time.
 * The stack fatures only a pointer overhead for each element
 * on the stack, this varies from platform to platform
 * ie 32-bits on 32-bit, 64-bits on 64-bit.
 * The implementation of the stack struct is supposed hidden,
 * so please use the functions for all your stack business.
 */
struct _stack_t;

/**
 * Initializes values in the stack. This is required to avoid
 * weird behavior.
 */
struct _stack_t* stack_open();
/**
 * Makes sure to release all memory allocated by the stack, before
 * you free the stack (or an auto variable goes out of scope).
 * After all relations from the stack to stack frames have been released,
 * the stack itself is freed.
 */
void stack_close(struct _stack_t*);
/**
 * Pops a specific stack, if the stack is null or the stack is
 * empty, null is returned.
 * Remember to free the output.
 */
char* stack_pop(struct _stack_t*);
/**
 * Pushes an object on top of the stack, if the stack is null,
 * 0 is returned, else 1 is returned.
 * The element is copied onto the struct, by using the char[1]
 * struct hack.
 */
int stack_push(struct _stack_t*, const char*);
/**
 * Returns the size of the stack, if the stack is empty, 0 is
 * returned.
 * If NULL is supplied, -1 is returned.
 */
int stack_size(struct _stack_t*);

#endif /* STACK_H_ */
