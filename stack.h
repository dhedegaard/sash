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
 */
struct _node_t;
typedef struct {
	struct _node_t *top;
	int size;
} _stack_t;

/**
 * Initializes values in the stack. This is required to avoid
 * weird behavior.
 */
void openstack(_stack_t *stack);
/**
 * Makes sure to release all memory allocated by the stack, before
 * you free the stack (or an auto variable goes out of scope).
 */
void closestack(_stack_t *stack);
/**
 * Pops a specific stack, if the stack is null or the stack is
 * empty, null is returned.
 * Remember to free the output.
 */
char* pop(_stack_t *stack);
/**
 * Pushes an object on top of the stack, if the stack is null,
 * 0 is returned, else 1 is returned.
 * The element is copied onto the struct, by using the char[1]
 * struct hack.
 */
int push(_stack_t *stack, const char *element);

#endif /* STACK_H_ */
