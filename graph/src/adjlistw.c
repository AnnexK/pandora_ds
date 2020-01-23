#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <limits.h>

#include "graph.h"
#include "iterator.h"
#include "list.h"

// list of struct _listdata
struct _graph
{
    list **adjlists;
    size_t v;
    unsigned char directed;
};

typedef struct _listdata
{
    size_t v;
    int weight;
} listdata;

int listdatacmp(const void *a, const void *b)
{
    size_t va = ((const listdata *)a)->v;
    size_t vb = *(size_t *)b;
    if (va < vb)
	return -1;
    else if (va > vb)
	return 1;
    else
	return 0;
}

graph *make_graph(size_t v_amt, unsigned char dir)
{
    graph *g = malloc(sizeof(graph));
    if (!g)
    {
	fprintf(stderr, "could not allocate struct\n");
	return NULL;
    }

    g->adjlists = malloc(v_amt * sizeof(list *));
    if (!g->adjlists)
    {
	fprintf(stderr, "could not allocate list array\n");
	free(g);
	return NULL;
    }

    for (size_t i = 0; i < v_amt; i++)
    {
	g->adjlists[i] = make_list(sizeof(listdata), listdatacmp);
    }

    g->directed = dir;
    g->v = v_amt;

    return g;
}

void dest_graph(graph *g)
{
    for (size_t i = 0; i < g->v; i++)
    {
	dest_list(g->adjlists[i]);
    }

    free(g->adjlists);
    free(g);
}

// data is weight
int add_edge(graph *g, size_t s, size_t e, int d)
{
    if (g->v <= s || g->v <= e)
    {
	fprintf(stderr, "no edge in graph\n");
	return EDGE_NOT_FOUND;
    }
    node *edge = search(g->adjlists[s], &e);

    if (edge)
    {
	// получить данные из найденного эл-та списка,
	// привести к типу указателя на данные списка
	// и записать новый вес
	((listdata *)data(edge))->weight = d;
    }
    else
    {
	listdata key = {e, d};
	insert(g->adjlists[s], NULL, &key);
    }
    return SUCCESS;
}

int get_edge(graph *g, size_t s, size_t e)
{
    node *edge;
    return g->v > s && g->v > e && (edge = search(g->adjlists[s], &e)) ? ((listdata *)data(edge))->weight : INT_MAX;
}

int remove_edge(graph *g, size_t s, size_t e)
{
    node *edge;
    if (g->v <= s || g->v <= e || !(edge = search(g->adjlists[s], &e)))
    {
	fprintf(stderr, "no edge in graph\n");
	return EDGE_NOT_FOUND;
    }
    delete(g->adjlists[s], edge);
    return SUCCESS;
}

struct _iterator
{
    size_t size;
    size_t vert;    
    node *cur;
};

iterator *make_iter(graph *g, size_t v)
{
    if (g->v <= v) return NULL;
    // пустой список
    if (!first(g->adjlists[v])) return NULL;
    iterator *it = malloc(sizeof(iterator));
    if (!it)
    {
	fprintf(stderr, "could not allocate struct\n");
	return NULL;
    }

    it->vert = v;
    it->cur = first(g->adjlists[v]);
    it->size = g->v;
    return it;
}

iterator *it_next(iterator *it)
{
    it->cur = next(it->cur);
    if (!it->cur)
    {
	free(it);
	return NULL;
    }
    return it;
}

size_t it_start(iterator *it)
{
    if (!it)
    {
	fprintf(stderr, "iterator exhausted\n");
	return it->size;
    }
    return it->vert;
}

size_t it_end(iterator *it)
{
    if (!it)
    {
	fprintf(stderr, "iterator exhausted\n");
	return it->size;
    }
    listdata *d = data(it->cur);
    return d->v;
}

int it_data(iterator *it)
{
    if (!it)
    {
	fprintf(stderr, "iterator exhausted\n");
	return INT_MAX;
    }
    listdata *d = data(it->cur);
    return d->weight;
}
