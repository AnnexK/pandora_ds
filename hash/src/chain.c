#include "hash.h"
#include "pair.h"
#include "list.h"

#include <stdlib.h>
#include <stdio.h>

struct _dict
{
    list **lists;
    size_t tab_size;
    hashf f;
};

pair *lst_search(list *l, K key)
{
    for (node *n = first(l); n; n = next(n))
    {
	pair *p = data(n);
	if (p->key == key)
	    return p;
    }
    return NULL;
}

dictionary *make_dict(size_t size, hashf f)
{
    dictionary *ret = malloc(sizeof(dictionary));
    if (!ret)
    {
	fprintf(stderr, "could not allocate struct\n");
	return NULL;
    }

    ret->tab_size = size;
    ret->f = f;

    ret->lists = malloc(size * sizeof(list *));
    for (size_t i = 0; i < size; i++)
    {
	ret->lists[i] = make_list();
    }

    return ret;
}

void dest_dict(dictionary *dct)
{
    for (size_t i = 0; i < dct->tab_size; i++)
	dest_list(dct->lists[i]);
    free(dct->lists);
    free(dct);
}

int insert(dictionary *dct, K key, V val)
{
    size_t h = dct->f(&key) % dct->tab_size;

    pair *p = lst_search(dct->lists[h], key);
    if (p)
    {
	p->value = val;
    }
    else
    {
	pair new = { key, val };
	linsert(dct->lists[h], NULL, new);
    }

    return 0;
}

pair *search(dictionary *dct, K key)
{
    size_t h = dct->f(&key) % dct->tab_size;
    return lst_search(dct->lists[h], key);
}

int delete(dictionary *dct, K key)
{
    size_t h = dct->f(&key) % dct->tab_size;

    for (node *n = first(dct->lists[h]); n; n = next(n))
    {
	if ( data(n)->key == key )
	{
	    ldelete(dct->lists[h], n);
	    return SUCCESS;
	}
    }
    return NOT_FOUND;
}

void print(dictionary *dct)
{
    for (size_t i = 0; i < dct->tab_size; i++)
    {
	if (!empty(dct->lists[i]))
	{
	    printf("List #%lu:\n", i);
	    for (node *n = first(dct->lists[i]); n; n = next(n))
	    {
		pprint(*data(n));
	    }
	}
    }
}
