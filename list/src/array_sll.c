#include "list.h"

#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define ALIGN(l) (offsetof(node, data) + (l)->data_size)

struct _node
{
    node *next;
    char data[];
};

struct _list
{
    void *arr;
    size_t data_size;
    size_t list_size;
    node *head;
    node *tail;
    node *free;
};

size_t list_size = 16384; // can be extern'd and changed

void *node_idx(list *l, size_t i)
{
    return (char *)(l->arr) + i*ALIGN(l);
}

list *make_list(size_t data_size)
{
    list *ret = malloc(sizeof(list));
    if (ret)
    {
	ret->data_size = data_size;
	ret->list_size = list_size;
	ret->arr = malloc(list_size * ALIGN(ret));
	if (!ret->arr)
	{
	    free(ret);
	    ret = NULL;
	}
	else
	{
	    ret->head = ret->tail = NULL;
	    ret->free = ret->arr;
	    char *n;
	    for (n = ret->arr; n < (char *)node_idx(ret, list_size-1); n += ALIGN(ret))
	    {
		((node *)n)->next = (node *)(n+ALIGN(ret));
	    }
	    ((node *)n)->next = NULL;
	}
    }
    return ret;
}

void dest_list(list *l)
{
    free(l->arr);
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

int insert(list *l, node *n, const void *data)
{
    node *new = l->free;
    if (!new) return INSERT_FAIL;

    l->free = l->free->next;

    memcpy(new->data, data, l->data_size);

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
	int cmpr = !cmp ? memcmp(h->data, d, l->data_size) : cmp(h->data, d);
	if (!cmpr)
	    break;
    }
    return h;
}

int delete(list *l, node *n)
{
    if (!n) return SUCCESS;
    if (n == l->head)
    {
	l->head = l->head->next;
	if (!l->head)
	    l->tail = NULL;
    }
    else
    {
	node *p = l->head;
	while (p && p->next != n)
	    p = p->next;
	if (!p)
	    return NODE_NOT_IN_LIST;
	p->next = n->next;
	if (n == l->tail)
	    l->tail = p;
    }
    n->next = l->free;
    l->free = n;
    return SUCCESS;
}

void map(list *l, void (*foo)(node *))
{
    for (node *n = l->head; n; n=n->next)
	foo(n);
}
