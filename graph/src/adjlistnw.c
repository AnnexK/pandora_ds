#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

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
const int no_edge = 0;

typedef struct _listdata
{
    size_t v;
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

graph *copy_graph(graph *g)
{
    if (!g) return NULL;
    graph *ret = make_graph(vertices(g), directed(g));
    if (!ret)
    {
	fprintf(stderr, "could not create graph struct\n");
	return NULL;
    }

    for (size_t i = 0; i < vertices(g); i++)
    {
	for (node *n = first(g->adjlists[i]); n; n = next(n))
	{
	    insert(ret->adjlists[i], last(ret->adjlists[i]), data(n));
	}
    }
    return ret;
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

size_t vertices(graph *g)
{
    return g->v;
}

unsigned char directed(graph *g)
{
    return g->directed;
}

// data is ignored
int add_edge(graph *g, size_t s, size_t e, int data)
{
    if (g->v <= s || g->v <= e)
    {
	fprintf(stderr, "no edge in graph\n");
	return EDGE_NOT_FOUND;
    }
    node *edge = search(g->adjlists[s], &e);

    if (!edge)
    {
	listdata key = {e};
	insert(g->adjlists[s], last(g->adjlists[s]), &key);
	if (!g->directed)
	{
	    key.v = s;
	    insert(g->adjlists[e], last(g->adjlists[s]), &key);
	}
    }
    return SUCCESS;
}

int get_edge(graph *g, size_t s, size_t e)
{
    return g->v > s && g->v > e && search(g->adjlists[s], &e) ? 1 : no_edge;
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
    if (!g->directed)
    {
	delete(g->adjlists[e], search(g->adjlists[e], &s));
    }
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
	return no_edge;
    }
    return 1;
}
