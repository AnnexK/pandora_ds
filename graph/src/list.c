#include "list.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// список модифицирован для использования с любыми примитивными типами
// или составными типами, состоящими из примитивных типов


struct _node
{
    void *data;
    struct _node *next;
};

struct _list
{
    struct _node *head;
    size_t ssize;
    cmpf f;
};

int belongs(list *l, node *n)
{
    if (!n) return 1;
    
    node *t = l->head;
    while (t && t != n)
	t = t->next;
    return t != NULL;
}

list *make_list(size_t ssize, cmpf f)
{
    list *ret = malloc(sizeof(list));
    if (!ret)
    {
	fprintf(stderr, "could not allocate struct\n");
	return NULL;
    }

    ret->ssize = ssize;
    ret->head = NULL;
    ret->f = f;
    
    return ret;
}

void dest_list(list *l)
{
    node *n = l->head;
    while (n)
    {
	node *tmp = n->next;
	free(n->data);
	free(n);
	n = tmp;
    }
    free(l);
}

node *first(list *l)
{
    return l->head;
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

int insert(list *l, node *n, void *d)
{
#ifdef CHECK_BELONG
    if (!belongs(l, n))
    {
	fprintf(stderr, "node does not belong to list\n");
	return NODE_NOT_IN_LIST;
    }
#endif
    node *new = malloc(sizeof(node));
    if (!new)
    {
	fprintf(stderr, "could not allocate node\n");
	return MEM_ERROR;
    }

    new->data = malloc(l->ssize);
    if (!new->data)
    {
	fprintf(stderr, "could not allocate data space\n");
	free(new);
	return MEM_ERROR;
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
    return SUCCESS;
}

node *search(list *l, void *key)
{
    // no compare function -- no comparisons
    if (!l->f) return NULL;
    for (node *n = l->head; n; n = n->next)
    {
	if ( l->f(n->data, key) == 0 )
	    return n;
    }
    return NULL;
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
    }
    else
    {
	node *p = l->head;
	while (p->next != n)
	    p = p->next;
	p->next = n->next;
    }
    free(n->data);
    free(n);
    return SUCCESS;
}

void map(list *l, void (*foo)(node *))
{
    for (node *n = l->head; n; n=n->next)
	foo(n);
}
