/*
 * queue.h
 *
 * Specification for a simple linked queue implementation.
 * The queue holds const char*.
 *
 *  Created on: 17/06/2010
 *      Author: Dennis Hedegaard
 */

#ifndef QUEUE_H_
#define QUEUE_H_

/**
 * Contains the internal queue structure, please use the functions
 * in this header to modify and use the queue.
 */
struct queue_t;

/**
 * Opens a new queue, returns a pointer to an empty queue.
 */
struct queue_t* queue_open();
/**
 * Attempts to close a queue, if successful, 1 is returned. Else 0.
 */
int queue_close(struct queue_t*);
/**
 * Attempts to poush the string to the back of the queue.
 * 1 is successful, else 0.
 */
int queue_enqueue(struct queue_t*, const char*);

/**
 * Polls the queue, if the queue is empty, NULL is returned.
 * This will remove the front node.
 * Remember to free the result, since it's on the heap.
 */
char*queue_poll(struct queue_t*);

/**
 * Gets the size of a queue, if there's an error -1 is returned.
 */
int queue_size(struct queue_t*);

#endif /* QUEUE_H_ */
