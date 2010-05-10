/*
 * stack.c
 *
 *  Created on: 16/03/2010
 *      Author: Dennis Hedegaard
 */

#include <stdlib.h>
#include <string.h>

#include "stack.h"

struct _stack_t {
	struct _node_t *top;
	int size;
};

struct _node_t {
	struct _node_t *next;
	char element[255];
};

struct _stack_t* openstack() {
	struct _stack_t *stack = malloc(sizeof(*stack));
	stack->size = 0;
	stack->top = NULL;
	return stack;
}

void closestack(struct _stack_t *stack) {
	while (stack->size > 0)
		pop(stack);
	free(stack);
}

char* pop(struct _stack_t *stack) {
	char* _e;
	struct _node_t *oldnode;
	int len = -1;
	if (stack == NULL || stack->size == 0)
		return NULL;
	len = strlen(stack->top->element);
	_e = malloc(len + 1);
	strncpy(_e, stack->top->element, len);
	*(_e + len) = '\0';
	oldnode = stack->top;
	stack->top = stack->top->next;
	free(oldnode);
	stack->size = stack->size - 1;
	return _e;
}

int push(struct _stack_t *stack, const char *element) {
	struct _node_t *newnode;
	int size = sizeof(*newnode) - 255 + 1 + strlen(element);
	if (stack == NULL)
		return 0;
	newnode = malloc(size);
	strcpy(newnode->element, element);
	newnode->next = stack->top;
	stack->top = newnode;
	stack->size++;
	return 1;
}

inline int stack_size(struct _stack_t *s) {
	if (s == NULL)
		return -1;
	return s->size;
}
