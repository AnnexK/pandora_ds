#include "list.h"

#include <stdlib.h>
#include <stddef.h>

#define ALIGN(l) (offsetof(node, data) + (l)->data_size)

struct _node
{
    size_t next;
    char[] data;
};

struct _list
{
    node *arr;
    size_t data_size;
    size_t list_size;
    size_t head;
    size_t free;
};

size_t list_size = 16384; // can be extern'd and changed

list *make_list(size_t data_size)
{
    list *ret = malloc(sizeof(list));
    if (ret)
    {
	ret->data_size = data_size;
	ret->list_size = list_size;
	ret->arr = malloc(list_size * ALIGN(l));
	ret->head = 0;
	
    }
}
