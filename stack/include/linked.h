#ifndef DS_LAB1_STACK
#define DS_LAB1_STACK

#include <stdlib.h>
#include <stdio.h>

#define STACK_MEM_ERROR 1
#define STACK_UNDERFLOW 2
#define STACK_SUCCESS 0

typedef struct _node
{
    int data;
    struct _node *next;
} node;

typedef struct _stack
{
    node *head;
} stack;

/* Stack ADT Interface */
stack *make_stack(void);
void dest_stack(stack *);

int push(stack *, int);
int pop(stack *, int *);
int peek(stack *, int *);
int empty(stack *);

/* Impl */

stack *make_stack(void)
{
    stack *s = malloc(sizeof(stack));
    if (!s)
    {
	fprintf(stderr, "couldn't allocate stack struct\n");
	return NULL;
    }
    s->head = NULL;
    return s;
}

void dest_stack(stack *S)
{
    int tmp;
    while (!empty(S))
    {
	pop(S, &tmp);
    }
    free(S);
}

int push(stack *S, int data)
{
    node *new = malloc(sizeof(node));
    if (!new)
    {
	fprintf(stderr, "couldn't allocate node struct\n");
	return STACK_MEM_ERROR;
    }

    new->data = data;
    new->next = S->head;
    S->head = new;
    return STACK_SUCCESS;
}

int pop(stack *S, int *ret)
{
    if (peek(S, ret) == STACK_SUCCESS)
    {
	node *rem = S->head;
	S->head = S->head->next;
	free(rem);
	return STACK_SUCCESS;
    }
    return STACK_UNDERFLOW;
}

int peek(stack *S, int *ret)
{
    if (empty(S))
    {
	fprintf(stderr, "Stack underflow\n");
	return STACK_UNDERFLOW;
    }
    *ret = S->head->data;
    return STACK_SUCCESS;
}

int empty(stack *S)
{
    return S->head == NULL;
}

#endif
