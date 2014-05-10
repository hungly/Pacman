#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

struct queue *queue_create(struct cell *cell)
{
	struct queue *queue = malloc(sizeof(struct queue));
	if (queue != NULL)
	{
		queue->cell = cell;
		queue->next = NULL;
		queue->size = 1;
	}
	return queue;
}

void enqueue(struct cell *cell, struct queue **queue)
{
	if (*queue != NULL)
	{
		if ((*queue)->size == 0)
		{
			(*queue)->next = queue_create(cell);
			(*queue)->size += 2;
			dequeue(queue);
		}
		else
		{
			if ((*queue)->next != NULL) enqueue(cell, &(*queue)->next);
			else
			{
				(*queue)->next = queue_create(cell);
				(*queue)->size += 1;
			}
		}
	}
}

struct cell *dequeue(struct queue **queue)
{
	if (*queue != NULL && (*queue)->size > 0)
	{
		struct cell *cell = (*queue)->cell;
		if ((*queue)->next != NULL)
		{
			(*queue)->size -= 1;
			struct queue *tmp_queue = *queue;
			*queue = (*queue)->next;
			free(tmp_queue);
		}
		else
		{
			(*queue)->size -= 1;
		}
		return cell;
	}
	else return NULL;
}

void queue_destroy(struct queue *queue)
{
	if (queue != NULL)
	{
		if (queue->next != NULL) queue_destroy(queue->next);
		free(queue);
	}
}

void queue_print(struct queue *queue)
{
	if (queue != NULL && queue->size > 0)
	{
		if (queue->cell != NULL)
		{
			printf("%d\t", queue->cell->value);
			if (queue->next != NULL) queue_print(queue->next);
		}
	}
}