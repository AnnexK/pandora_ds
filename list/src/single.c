#include "list.h"

#include <stdlib.h>
#include <stdio.h>

struct _node
{
    T data;
    struct _node *next;
};

struct _list
{
    struct _node *head;
};

int belongs(list *l, node *n)
{
    if (!n) return 1;
    
    node *t = l->head;
    while (t && t != n)
	t = t->next;
    return t != NULL;
}

list *make_list(void)
{
    list *ret = malloc(sizeof(list));

    if (!ret)
    {
	fprintf(stderr, "could not allocate struct");
	return NULL;
    }

    ret->head = NULL;
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

T *data(node *n)
{
    return &(n->data);
}

node *next(node *n)
{
    return n->next;
}

int empty(list *l)
{
    return l->head == NULL;
}

int insert(list *l, node *n, T d)
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

    new->data = d;
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

node *search(list *l, T d)
{
    node *h;
    for (h = l->head; h; h = h->next)
    {
	if (h->data == d)
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
    }
    else
    {
	node *p = l->head;
	while (p->next != n)
	    p = p->next;
	p->next = n->next;
    }
    free(n);
    return SUCCESS;
}

void map(list *l, void (*foo)(node *))
{
    for (node *n = l->head; n; n=n->next)
	foo(n);
}
