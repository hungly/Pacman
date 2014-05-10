#ifndef QUEUE
#define QUEUE

#include "cell.h"

struct queue
{
	struct cell *cell;
	struct queue *next;
	int size;
};

struct queue *queue_create(struct cell *cell);

void enqueue(struct cell *cell, struct queue **queue);

struct cell *dequeue(struct queue **queue);

void queue_destroy(struct queue *queue);

void queue_print(struct queue *queue);

#endif