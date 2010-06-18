/*
 * queue.c
 *
 *  Created on: 17/06/2010
 *      Author: Dennis Hedegaard
 */

#include <stdlib.h>
#include <string.h>

#include "queue.h"

struct queue_node_t {
	struct queue_note_t *next;
	char e[255];
};

struct queue_t {
	int size;
	struct queue_node_t *front;
	struct queue_node_t *back;
};

struct queue_t* queue_open() {
	struct queue_t *q = malloc(sizeof(*q));
	q->back = NULL;
	q->front = NULL;
	q->size = 0;
	return q;
}

int queue_close(struct queue_t *q) {
	if (!q)
		return 0;
	while (q->front) {
		struct queue_node_t *n = q->front;
		q->front = n->next;
		free(n);
	}
	return 1;
}

int queue_enqueue(struct queue_t *q, const char *str) {
	struct queue_node_t *n = NULL;
	int size = 0;
	if (!q)
		return 0;
	size = strlen(str) + 1;
	n = malloc(sizeof(*n) - 255 + size);
	n->next = NULL;
	strncpy(n->e, str, size - 1);
	*(n->e + size - 1) = '\0';
	if (q->back)
		q->back->next = n;
	q->back = n;
	if (!q->front && !q->back) {
		q->front = n;
		q->back = n;
	}
	q->size++;
	return 1;
}

char*queue_poll(struct queue_t *q) {
	char* e = NULL;
	struct queue_node_t *old;
	int len = -1;
	if (!q || q->size == 0)
		return NULL;
	old = q->front;
	len = strlen(old->e);
	e = malloc(len + 1);
	strncpy(e, old->e, len);
	*(e + len) = '\0';
	q->front = old->next;
	free(old);
	q->size--;
	return e;
}

inline int queue_size(struct queue_t *q) {
	return q ? q->size : -1;
}
