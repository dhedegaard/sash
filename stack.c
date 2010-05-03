/*
 * stack.c
 *
 *  Created on: 16/03/2010
 *      Author: Dennis Hedegaard
 */

#include <stdlib.h>
#include <string.h>

#include "stack.h"

struct _node_t {
	struct _node_t *next;
	char element[1];
};

void openstack(_stack_t *stack) {
	if (stack == NULL)
		return;
	stack->size = 0;
	stack->top = NULL;
}

void closestack(_stack_t *stack) {
	while (stack->size > 0)
		pop(stack);
}

const char* pop(_stack_t *stack) {
	const char* _e;
	struct _node_t *oldnode;
	if (stack == NULL || stack->size == 0)
		return NULL;
	_e = stack->top->element;
	oldnode = stack->top;
	stack->top = stack->top->next;
	free(oldnode);
	stack->size = stack->size - 1;
	return _e;
}

int push(_stack_t *stack, const char *element) {
	struct _node_t *newnode;
	if (stack == NULL)
		return 0;
	newnode = malloc(sizeof(*newnode) + strlen(element));
	strcpy(newnode->element, element);
	newnode->next = stack->top;
	stack->top = newnode;
	stack->size = stack->size + 1;
	return 1;
}
