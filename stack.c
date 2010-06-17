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
	struct stack_node_t *top;
	int size;
};

struct stack_node_t {
	struct stack_node_t *next;
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
	struct stack_node_t *oldnode;
	int len = -1;
	if (stack == NULL || stack->size == 0)
		return NULL;
	len = strlen(stack->top->element);
	_e = malloc(len + 1);
	strncpy(_e, stack->top->element, len);
	*(_e + len) = '\0';
	oldnode = stack->top;
	stack->top = oldnode->next;
	free(oldnode);
	stack->size--;
	return _e;
}

int push(struct _stack_t *stack, const char *element) {
	struct stack_node_t *newnode;
	int size = strlen(element) + 1;
	if (stack == NULL)
		return 0;
	newnode = malloc(sizeof(*newnode) - 255 + size);
	strncpy(newnode->element, element, size - 1);
	*(newnode->element + size - 1) = '\0';
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
