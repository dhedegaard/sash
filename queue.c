/*
 * queue.c
 *
 *  Created on: 17/06/2010
 *      Author: Dennis Hedegaard
 */

#include <stdlib.h>
#include <string.h>

#include "queue.h"

struct queue_t {
	int size;
	struct queue_node_t *front;
	struct queue_node_t *back;
};

struct queue_node_t {
	struct queue_note_t *next;
	char e[255];
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
		q->front = q->front->next;
		free(n);
	}
	return 1;
}

int queue_enqueue(struct queue_t *q, const char *str) {

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

int queue_size(struct queue_t *q) {
	return q ? q->size : -1;
}
