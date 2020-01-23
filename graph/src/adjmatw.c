#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <limits.h>

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
	g->mat[i] = INT_MAX;
    }
    
    g->v = v_amt;
    g->directed = dir;
    return g;
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

    g->mat[s*g->v+e] = INT_MAX;
    if (!g->directed)
    {
	g->mat[e*g->v+s] = INT_MAX;
    }
    return SUCCESS;
}

int get_edge(graph *g, size_t s, size_t e)
{
    if (g->v <= s || g->v <= e)
    {
	// fprintf(stderr, "no edge in graph\n");
	return INT_MAX;
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

    return *(it->row) == INT_MAX ? it_next(it) : it;
}

iterator *it_next(iterator *it)
{
    if (!it) return NULL;
    for (size_t i = it->cur+1; i < it->size; i++)
    {
	if (it->row[i] != INT_MAX)
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
	return INT_MAX;
    }
    return it->row[it->cur];
}
