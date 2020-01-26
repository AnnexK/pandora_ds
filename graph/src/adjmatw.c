#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <limits.h>
#include <string.h>

#include "graph.h"
#include "iterator.h"

// not INT_MAX -- edge present
// INT_MAX -- edge not present

struct _graph
{
    int *mat;
    size_t v;
    unsigned char directed;
};
const int no_edge = INT_MAX;

graph *make_graph(size_t v_amt, unsigned char dir)
{
    graph *g = malloc(sizeof(graph));
    if (!g)
    {
	fprintf(stderr, "could not allocate struct\n");
	return NULL;
    }

    g->mat = malloc(v_amt * v_amt * sizeof(int));
    if (!g->mat)
    {
	fprintf(stderr, "could not allocate matrix\n");
	free(g);
	return NULL;
    }
    
    for (size_t i = 0; i < v_amt * v_amt; i++)
    {
	g->mat[i] = no_edge;
    }
    
    g->v = v_amt;
    g->directed = dir;
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

    memcpy(ret->mat, g->mat, vertices(g)*vertices(g)*sizeof(int));
    return ret;
}

void dest_graph(graph *g)
{
    free(g->mat);
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

// data is weight
int add_edge(graph *g, size_t s, size_t e, int data)
{
    if (g->v <= s || g->v <= e)
    {
	fprintf(stderr, "no edge in graph\n");
	return EDGE_NOT_FOUND;
    }

    g->mat[s*g->v+e] = data;
    if (!g->directed)
    {
	g->mat[e*g->v+s] = data;
    }
    return SUCCESS;
}

int remove_edge(graph *g, size_t s, size_t e)
{
    if (g->v <= s || g->v <= e)
    {
	fprintf(stderr, "no edge in graph\n");
	return EDGE_NOT_FOUND;
    }

    g->mat[s*g->v+e] = no_edge;
    if (!g->directed)
    {
	g->mat[e*g->v+s] = no_edge;
    }
    return SUCCESS;
}

int get_edge(graph *g, size_t s, size_t e)
{
    if (g->v <= s || g->v <= e)
    {
	return no_edge;
    }

    return g->mat[s*g->v+e];
}

struct _iterator
{
    size_t vert;
    int *row;
    size_t size;
    size_t cur;
};

iterator *make_iter(graph *g, size_t v)
{
    if (g->v <= v) return NULL;
    iterator *it = malloc(sizeof(iterator));
    if (!it)
    {
	fprintf(stderr, "could not allocate struct\n");
	return NULL;
    }

    it->vert = v;
    it->row = g->mat + v*g->v;
    it->size = g->v;
    it->cur = 0;

    return *(it->row) == no_edge ? it_next(it) : it;
}

iterator *it_next(iterator *it)
{
    if (!it) return NULL;
    for (size_t i = it->cur+1; i < it->size; i++)
    {
	if (it->row[i] != no_edge)
	{
	    it->cur = i;
	    return it;
	}
    }
    free(it);
    return NULL;
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
    return it->cur;
}

int it_data(iterator *it)
{
    if (!it)
    {
	fprintf(stderr, "iterator exhausted\n");
	return no_edge;
    }
    return it->row[it->cur];
}
