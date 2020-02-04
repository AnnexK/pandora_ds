#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include <string.h>

#include "graph.h"
#include "iterator.h"

struct _graph
{
    double *mat;
    size_t v;
    unsigned char directed;
};
const double no_edge = INFINITY;

graph *make_graph(size_t v_amt, unsigned char dir)
{
    graph *g = malloc(sizeof(graph));
    if (!g)
    {
	fprintf(stderr, "could not allocate struct\n");
	return NULL;
    }

    g->mat = malloc(v_amt * v_amt * sizeof(double));
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

    memcpy(ret->mat, g->mat, vertices(g)*vertices(g)*sizeof(double));
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
int add_edge(graph *g, size_t s, size_t e, double data)
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

double get_edge(graph *g, size_t s, size_t e)
{
    if (g->v <= s || g->v <= e)
    {
	return no_edge;
    }

    return g->mat[s*g->v+e];
}

void transpose(graph *g)
{
    if (directed(g))
    {
	size_t v = vertices(g);
	for (size_t i = 0; i < v-1; i++)
	{
	    for (size_t j = i+1; j < v; j++)
	    {
		double tmp = g->mat[i*v+j];
		g->mat[i*v+j] = g->mat[j*v+i];
		g->mat[j*v+i] = tmp;
	    }
	}
    }
}

double *make_adj_mat(graph *g)
{
    double *ret = malloc(sizeof(double) * vertices(g) * vertices(g));
    memcpy(ret, g->mat, sizeof(double)*vertices(g)*vertices(g));
    return ret;
}

struct _iterator
{
    size_t vert;
    double *row;
    size_t size;
    size_t cur;
};

iterator *make_iter(void)
{
    return malloc(sizeof(iterator));
}

void set_invalid(iterator *it)
{
    if (it)
	it->size = it->cur = 0;
}

void it_init(iterator *it, graph *g, size_t v)
{
    if (!it) return;
    if (g->v <= v)
    {
	set_invalid(it);
	return;
    }

    it->vert = v;
    it->row = g->mat + v*g->v;
    it->size = g->v;
    it->cur = 0;

    if (*(it->row) == no_edge)
	it_next(it);
}

void it_next(iterator *it)
{
    if (!it_valid(it)) return;
    for (it->cur++; it->cur < it->size; it->cur++)
    {
	if (it->row[it->cur] != no_edge)
	{
	    return;
	}
    }
}

size_t it_start(iterator *it)
{
    if (!it_valid(it))
    {
	fprintf(stderr, "iterator exhausted\n");
	return it->size;
    }
    return it->vert;
}

size_t it_end(iterator *it)
{
    if (!it_valid(it))
    {
	fprintf(stderr, "iterator exhausted\n");
	return it->size;
    }
    return it->cur;
}

double it_data(iterator *it)
{
    if (!it_valid(it))
    {
	fprintf(stderr, "iterator exhausted\n");
	return no_edge;
    }
    return it->row[it->cur];
}

int it_valid(iterator *it)
{
    return it && it->cur < it->size;
}
