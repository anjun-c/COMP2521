// Interface to the Queue ADT

// !!! DO NOT MODIFY THIS FILE !!!

#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <stdio.h>

#include "cell.h"

typedef struct cell Item;

typedef struct queue *Queue;

/**
 * Creates a new empty queue
 * Time complexity: O(1)
 */
Queue QueueNew(void);

/**
 * Frees all memory allocated to the queue
 * Time complexity: O(n)
 */
void QueueFree(Queue q);

/**
 * Adds an item to the end of the queue
 * Time complexity: O(1)
 */
void QueueEnqueue(Queue q, Item it);

/**
 * Removes an item from the front of the queue and returns it
 * Assumes that the queue is not empty
 * Time complexity: O(1)
 */
Item QueueDequeue(Queue q);

/**
 * Gets the item at the front of the queue without removing it
 * Assumes that the queue is not empty
 * Time complexity: O(1)
 */
Item QueueFront(Queue q);

/**
 * Gets the size of the given queue
 * Time complexity: O(1)
 */
int QueueSize(Queue q);

/**
 * Returns true if the queue is empty, and false otherwise
 * Time complexity: O(1)
 */
bool QueueIsEmpty(Queue q);

/**
 * Prints the queue to the given file with items space-separated
 * Time complexity: O(n)
 */
void QueueDump(Queue q, FILE *fp);

#endif

