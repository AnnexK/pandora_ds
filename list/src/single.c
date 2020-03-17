#include "list.h"

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

struct _node
{
    struct _node *next;
    char data[];
};

struct _list
{
    size_t ssize;
    struct _node *head;
    struct _node *tail;
};

// type(L) = (struct list *)
#define NSIZE(L) (sizeof(struct _node) + (L)->ssize)

int belongs(list *l, node *n)
{
    if (!n) return 1;
    
    node *t = l->head;
    while (t && t != n)
	t = t->next;
    return t != NULL;
}

list *make_list(size_t ssize)
{
    if (!ssize)
    {
	fprintf(stderr, "cannot hold data of size 0\n");
	return NULL;
    }
    
    list *ret = malloc(sizeof(list));

    if (!ret)
    {
	fprintf(stderr, "could not allocate struct");
	return NULL;
    }

    ret->head = ret->tail = NULL;
    ret->ssize = ssize;
    return ret;
}

void dest_list(list *l)
{
    node *n = l->head;
    while (n)
    {
	node *tmp = n->next;
	free(n);
	n = tmp;
    }
    free(l);
}

node *first(list *l)
{
    return l->head;
}

node *last(list *l)
{
    return l->tail;
}

void *data(node *n)
{
    return n->data;
}

node *next(node *n)
{
    return n->next;
}

int empty(list *l)
{
    return l->head == NULL;
}

int insert(list *l, node *n, const void *d)
{
#ifdef CHECK_BELONG
    if (!belongs(l, n))
    {
	fprintf(stderr, "node does not belong to list\n");
	return NODE_NOT_IN_LIST;
    }
#endif
    node *new = malloc(NSIZE(l));
    if (!new)
    {
	fprintf(stderr, "could not allocate node\n");
	return INSERT_FAIL;
    }

    memcpy(new->data, d, l->ssize);
    if (n)
    {
	new->next = n->next;
	n->next = new;
    }
    else
    {
	new->next = l->head;
	l->head = new;
    }
    if (n == l->tail)
    {
	l->tail = new;
    }
    
    return SUCCESS;
}

node *search(list *l, const void *d, int (*cmp)(const void *, const void *))
{
    node *h;
    for (h = l->head; h; h = h->next)
    {
	int cmpr = !cmp ? memcmp(h->data, d, l->ssize) : cmp(h->data, d);
	if (!cmpr)
	    break;
    }
    return h;
}

int delete(list *l, node *n)
{
    if (!n) return SUCCESS;
#ifdef CHECK_BELONG
    if (!belongs(l, n))
    {
	fprintf(stderr, "node does not belong to list\n");
	return NODE_NOT_IN_LIST;
    }
#endif

    if (n == l->head)
    {
	l->head = l->head->next;
	if (!l->head)
	{
	    l->tail = NULL;
	}	
    }
    else
    {
	node *p = l->head;
	while (p->next != n)
	    p = p->next;
	p->next = n->next;
	if (n == l->tail)
	{
	    l->tail = p;
	}
    }
    free(n);
    return SUCCESS;
}

void map(list *l, void (*foo)(node *))
{
    for (node *n = l->head; n; n=n->next)
	foo(n);
}
