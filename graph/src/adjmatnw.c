#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "graph.h"
#include "iterator.h"
#include "list.h"

struct _graph
{
    unsigned char *mat;
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

    g->mat = malloc(v_amt * v_amt * sizeof(unsigned char));
    for (size_t i = 0; i < v_amt*v_amt; i++)
    {
	g->mat[i] = 0;
    }
    
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

graph *copy_graph(graph *g)
{
    if (!g) return NULL;
    graph *ret = make_graph(vertices(g), directed(g));
    if (!ret)
    {
	fprintf(stderr, "could not create graph struct\n");
	return NULL;
    }

    memcpy(ret->mat, g->mat, vertices(g)*vertices(g)*sizeof(unsigned char));
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

// data ignored
int add_edge(graph *g, size_t s, size_t e, double data)
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

double get_edge(graph *g, size_t s, size_t e)
{
    if (g->v <= s || g->v <= e)
    {
	return no_edge;
    }

    return g->mat[s*g->v+e] ? 1.0 : no_edge;
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
		unsigned char tmp = g->mat[i*v+j];
		g->mat[i*v+j] = g->mat[j*v+i];
		g->mat[j*v+i] = tmp;
	    }
	}
    }
}

double *make_adj_mat(graph *g)
{
    double *ret = malloc(sizeof(double) * vertices(g) * vertices(g));
    for (size_t i = 0; i < vertices(g) * vertices(g); i++)
	ret[i] = g->mat[i] ? 1.0 : no_edge;
    
    return ret;
}

struct _iterator
{
    size_t vert;
    unsigned char *row;
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

    if (!it->row[it->cur])
	it_next(it);
}

void it_next(iterator *it)
{
    if (!it_valid(it)) return;
    for (it->cur++; it->cur < it->size; it->cur++)
    {
	if (it->row[it->cur])
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
    return 1.0;
}

int it_valid(iterator *it)
{
    return it && it->cur < it->size;
}

const unsigned char *warshall_mat(graph *g)
{
    size_t v = vertices(g);
    for (size_t k = 0; k < v; k++)
	for (size_t i = 0; i < v; i++)
	    for (size_t j = 0; j < v; j++)
		g->mat[i*v+j] = g->mat[i*v+j] || (g->mat[i*v+k] && g->mat[k*v+j]);
    return g->mat;
}
