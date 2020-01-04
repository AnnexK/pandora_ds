#ifndef DS_LAB1_STACK
#define DS_LAB1_STACK

#include <stdlib.h>
#include <stdio.h>

#define STACK_OVERFLOW 1
#define STACK_UNDERFLOW 2
#define STACK_SUCCESS 0

typedef struct _stack
{
    int *data;
    size_t max_size;
    size_t current;
} stack;

/* Stack ADT Interface */
stack *make_stack(size_t);
void dest_stack(stack *);

int push(stack *, int);
int pop(stack *, int *);
int peek(stack *, int *);
int empty(stack *);

/* Impl */
stack *make_stack(size_t size)
{
    stack *s = malloc(sizeof(stack));
    if (!s)
    {
	fprintf(stderr, "couldn't allocate stack struct");
	return NULL;
    }
    s->data = malloc(size * sizeof(int));
    if (!(s->data))
    {
	fprintf(stderr, "couldn't allocate data storage\n");
	free(s);
	return NULL;
    }
    s->max_size = size;
    s->current = 0;
    return s;
}

void dest_stack(stack *S)
{
    free(S->data);
    free(S);    
}

int push(stack *S, int data)
{
    if (S->current == S->max_size)
    {
	fprintf(stderr, "Stack overflow\n");
	return STACK_OVERFLOW;
    }

    S->data[S->current++] = data;
    return STACK_SUCCESS;
}

int pop(stack *S, int *ret)
{
    if (peek(S, ret) == STACK_SUCCESS)
    {
	S->current--;
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

    *ret = S->data[S->current-1];
    return STACK_SUCCESS;
}

int empty(stack *S)
{
    return S->current == 0;
}

#endif
