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
    struct _node *tail;
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
    ret->head = ret->tail = NULL;
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
    if (!n) return NULL;
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
    if (n == l->tail)
    {
	l->tail = new;
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

    // p -- узел списка, находящийся перед n    
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
    free(n->data);
    free(n);
    return SUCCESS;
}

void map(list *l, void (*foo)(node *))
{
    for (node *n = l->head; n; n=n->next)
	foo(n);
}


void get_sent_nodes(node *, size_t, node **, node **);
node *merge(node *, node *, node *, cmpf);

void lsort(list *l, cmpf f)
{
    size_t list_size = 0;

    for (node *n = l->head; n; n = n->next)
	list_size++;

    for (size_t sub_size = 1; sub_size < list_size; sub_size *= 2)
    {
	node *left = l->head;
	node *mid, *right;
	while (left)
	{
	    get_sent_nodes(left, sub_size, &mid, &right);
	    node *tmp = merge(left, mid, right, f);
	    if (left == l->head)
		l->head = tmp;
	    left = right;
	}
    }    
}

void get_sent_nodes(node *left, size_t ssize, node **mid, node **right)
{
    *mid = *right = left;

    for (size_t i = 0; i < ssize; i++)
    {
	*mid = next(*mid);
	*right = next(next(*right));
    }
}

node *merge(node *left, node *mid, node *right, cmpf f)
{
    static node *prev = NULL;
    node *first = left;
    node *second = mid;

    if (!second)
    {
	prev = NULL;
	return left;
    }
    node *temp = NULL;
    node *head = NULL;

    while (first != mid && second != right)
    {
	node *min = f(first->data, second->data) < 0 ? first : second;
	if (min == first)
	    first = first->next;
	else
	    second = second->next;
	
	if (!temp)
	{
	    temp = min;
	    head = min;
	}
	else
	{
	    temp->next = min;
	    temp = temp->next;
	}
    }
    
    temp->next = first != mid ? first : second;
    node *barrier = first != mid ? mid : right;

    while (temp->next != barrier)
	temp = temp->next;
    temp->next = right;

    if (prev)
	prev->next = head;

    prev = right ? temp : right;
    return head;
}
