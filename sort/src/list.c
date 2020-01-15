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

node *first(list *l)
{
    return l->head;
}

T *data(node *n)
{
    return &(n->data);
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

void get_sent_nodes(node *, size_t, node **, node **);
node *merge(node *, node *, node *);

void lsort(list *l)
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
	    node *tmp = merge(left, mid, right);
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

node *merge(node *left, node *mid, node *right)
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
	node *min = first->data < second->data ? first : second;
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
