#ifndef DS_LAB2_QUEUE
#define DS_LAB2_QUEUE

#include <stdlib.h>
#include <stdio.h>

#define QUEUE_MEM_ERROR 1
#define QUEUE_UNDERFLOW 2
#define QUEUE_SUCCESS 0

typedef struct _node
{
    unsigned int data;
    struct _node *next;
} node;

typedef struct _queue
{
    node *head;
    node *tail;
} queue;

/* Queue ADT Interface */
queue *make_queue(void);
void dest_queue(queue *);

int enqueue(queue *, unsigned int);
int dequeue(queue *, unsigned int *);
int peek(queue *, unsigned int *);
int empty(queue *);

/* Impl */

queue *make_queue(void)
{
    queue *q = malloc(sizeof(queue));
    if (!q)
    {
	fprintf(stderr, "couldn't allocate queue struct");
	return NULL;
    }

    q->head = q->tail = NULL;
    return q;
}

void dest_queue(queue *Q)
{
    unsigned int tmp;
    while (!empty(Q))
    {
	dequeue(Q, &tmp);
    }
    free(Q);
}

int enqueue(queue *Q, unsigned int data)
{
    node *new = malloc(sizeof(node));
    if (!new)
    {
	fprintf(stderr, "couldn't allocate node struct");
	return QUEUE_MEM_ERROR;
    }
    new->data = data;
    new->next = NULL;
    if (Q->tail)
    {
	Q->tail->next = new;
    }
    else
    {
	Q->head = new;
    }
    Q->tail = new;
    return QUEUE_SUCCESS;
}

int dequeue(queue *Q, unsigned int *ret)
{
    if (peek(Q, ret) == QUEUE_SUCCESS)
    {
	node *new_head = Q->head->next;
	free(Q->head);
	Q->head = new_head;
	if (!Q->head)
	{
	    Q->tail = NULL;
	}
	return QUEUE_SUCCESS;
    }
    return QUEUE_UNDERFLOW;
}

int peek(queue *Q, unsigned int *ret)
{
    if (empty(Q))
    {
	fprintf(stderr, "queue underflow");
	return QUEUE_UNDERFLOW;
    }

    *ret = Q->head->data;
    return QUEUE_SUCCESS;
}

int empty(queue *Q)
{
    return Q->head == NULL;
}

#endif
