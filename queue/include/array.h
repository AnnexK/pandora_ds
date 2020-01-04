#ifndef DS_L2_QUEUE
#define DS_L2_QUEUE

#include <stdlib.h>
#include <stdio.h>

#define ENQUEUE 0
#define DEQUEUE 255

#define QUEUE_OVERFLOW 1
#define QUEUE_UNDERFLOW 2
#define QUEUE_SUCCESS 0

typedef struct _queue
{
    int *data;
    size_t size;
    size_t head, tail;
    unsigned char last;
} queue;

/* Queue ADT Interface */
queue *make_queue(size_t);
void dest_queue(queue *);

int enqueue(queue *, int);
int dequeue(queue *, int *);
int peek(queue *, int *);
int empty(queue *);

/* Impl */

queue *make_queue(size_t size)
{
    queue *q = malloc(sizeof(queue));
    if (!q)
    {
	fprintf(stderr, "couldn't allocate queue struct");
	return NULL;
    }

    q->data = malloc(size * sizeof(int));
    if (!(q->data))
    {
	fprintf(stderr, "couldn't allocate data buffer");
	free(q);
	return NULL;
    }

    q->head = q->tail = 0;
    q->size = size;
    q->last = DEQUEUE;

    return q;
}

void dest_queue(queue *Q)
{
    free(Q->data);
    free(Q);
}

int enqueue(queue *q, int data)
{
    if (q->head == q->tail && q->last == ENQUEUE)
    {
	fprintf(stderr, "queue overflow");
	return QUEUE_OVERFLOW;
    }

    q->data[q->tail] = data;
    q->tail = (q->tail + 1) % q->size;
    q->last = ENQUEUE;
    
    return QUEUE_SUCCESS;
}

int dequeue(queue *q, int *ret)
{
    if (peek(q, ret) == QUEUE_SUCCESS)
    {
	q->head = (q->head + 1) % q->size;
	q->last = DEQUEUE;
	return QUEUE_SUCCESS;
    }
    return QUEUE_UNDERFLOW;
}

int peek(queue *q, int *ret)
{
    if (empty(q))
    {
	fprintf(stderr, "queue underflow");
	return QUEUE_UNDERFLOW;
    }

    *ret = q->data[q->head];
    return QUEUE_SUCCESS;
}

int empty(queue *q)
{
    return (q->head == q->tail && q->last == DEQUEUE);
}

#endif
