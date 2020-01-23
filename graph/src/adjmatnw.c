#include <stdlib.h>
#include <stdio.h>

#include "graph.h"
#include "iterator.h"
#include "list.h"

// 1 -- edge present
// 0 -- edge not present

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

    g->mat = calloc(v_amt * v_amt, sizeof(int));
    if (!g->mat)
    {
	fprintf(stderr, "could not allocate matrix\n");
	free(g);
	return NULL;
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

// data ignored
int add_edge(graph *g, size_t s, size_t e, int data)
{
    if (g->v <= s || g->v <= e)
    {
	fprintf(stderr, "no edge in graph\n");
	return EDGE_NOT_FOUND;
    }

    g->mat[s*g->v+e] = 1;
    if (!g->directed)
    {
	g->mat[e*g->v+s] = 1;
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

    g->mat[s*g->v+e] = 0;
    if (!g->directed)
    {
	g->mat[e*g->v+s] = 0;
    }
    return SUCCESS;
}

int get_edge(graph *g, size_t s, size_t e)
{
    if (g->v <= s || g->v <= e)
    {
	// fprintf(stderr, "no edge in graph\n");
	return 0;
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

    return *(it->row) ? it : it_next(it);
}

iterator *it_next(iterator *it)
{
    if (!it) return NULL;
    for (size_t i = it->cur+1; i < it->size; i++)
    {
	if (it->row[i])
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
	return 0;
    }
    return 1;
}
