#include "hash.h"
#include "pair.h"
#include "dhash.h"

#include <stdlib.h>
#include <stdio.h>

struct _dict
{
    pair *tab;
    unsigned char *exists;
    size_t tab_size;
    hashf f1;
    hashf f2;
};

unsigned int tries;

dictionary *make_dict(size_t size, hashf f1, hashf f2)
{
    dictionary *ret = malloc(sizeof(dictionary));
    if (!ret)
    {
	fprintf(stderr, "could not allocate struct\n");
	return NULL;
    }

    ret->tab = malloc(size * sizeof(pair));
    ret->exists = calloc(size, sizeof(pair));
    if (!ret->tab || !ret->exists)
    {
	fprintf(stderr, "could not allocate tabs\n");
	dest_dict(ret);
	return NULL;
    }

    ret->tab_size = size;
    ret->f1 = f1;
    ret->f2 = f2;
    return ret;
}

void dest_dict(dictionary *dict)
{
    if (dict)
    {
	free(dict->tab);
	free(dict->exists);
	free(dict);
    }
}

int insert(dictionary *dict, K key, V val)
{
    tries = 1;
    
    size_t h = dict->f1(&key) % dict->tab_size;
    size_t h2 = dict->f2(&key) % dict->tab_size;
    
    while (dict->exists[h] && dict->tab[h].key != key && tries < dict->tab_size)
    {
	tries++;
	h = (h+h2) % dict->tab_size;
    }

    if (tries == dict->tab_size)
	return NOT_FOUND;
    
    dict->tab[h].key = key;
    dict->tab[h].value = val;
    dict->exists[h] = 1;

    return SUCCESS;
}

pair *search(dictionary *dict, K key)
{
    tries = 1;

    size_t h = dict->f1(&key) % dict->tab_size;
    size_t h2 = dict->f2(&key) % dict->tab_size;
    
    while (dict->exists[h] && dict->tab[h].key != key && tries < dict->tab_size)
    {
	tries++;
	h = (h+h2) % dict->tab_size;
    }

    if (dict->exists[h] && dict->tab[h].key == key)
	return dict->tab + h;
    else
	return NULL;
}

int delete(dictionary *dict, K key)
{
    tries = 0;
    return NOT_IMPLEMENTED;
}

void print(dictionary *dict)
{
    for (size_t i = 0; i < dict->tab_size; i++)
    {
	if (dict->exists[i])
	{
	    printf("Cell #%lu:\n", i);
	    pprint(dict->tab[i]);
	}
    }
}

unsigned int get_last_tries(void)
{
    return tries;
}
