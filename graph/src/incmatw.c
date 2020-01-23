#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <limits.h>
#include <string.h>

#include "graph.h"
#include "iterator.h"


// (v+1) items in matrix vector
// to store weight

#define ALLOC_SIZE 100

struct _graph
{
    int *mat;
    size_t v;
    size_t edge_capacity;
    size_t edge_current;
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

    g->mat = NULL;
    g->edge_current = 0;
    g->edge_capacity = 0;
    g->v = v_amt;
    g->directed = dir;

    return g;
}

void dest_graph(graph *g)
{
    free(g->mat);
    free(g);
}

int *edge_lookup(graph *g, size_t s, size_t e)
{
    int type = g->directed && s != e ? -1 : 1;
    for (size_t i = 0; i < g->edge_current; i++)
    {
	if (g->mat[i*(g->v+1)+s] && g->mat[i*(g->v+1)+e] == type)
	    return g->mat+i*(g->v+1);
    }
    return NULL;
}

// data is weight
int add_edge(graph *g, size_t s, size_t e, int data)
{
    if (g->v <= s || g->v <= e)
    {
	fprintf(stderr, "no edge in graph\n");
	return EDGE_NOT_FOUND;
    }
    int *needle;
    if ((needle = edge_lookup(g, s, e)))
    {
        needle[g->v] = data;
    }
    else
    {
	if (g->edge_capacity == g->edge_current)
	{
	    int *tmp = realloc(g->mat, (g->edge_current+ALLOC_SIZE)*(g->v+1));
	    if (!tmp)
	    {
		fprintf(stderr, "could not reallocate incidence matrix\n");
		return ALLOC_ERROR;
	    }
	    
	    g->mat = tmp;
	    g->edge_capacity += ALLOC_SIZE;
	}
	for (size_t i = 0; i < g->v; i++)
	{
	    if (i != s && i != e)
		g->mat[g->edge_current*(g->v+1)+i] = 0;
	}

	g->mat[g->edge_current*(g->v+1)+s] = 1;
	// s == e -- петля, поэтому на e пишем 1
	g->mat[g->edge_current*(g->v+1)+e] = g->directed && s != e ? -1 : 1;
	g->mat[g->edge_current*(g->v+1)+g->v] = data;
	g->edge_current++;
    }
    return SUCCESS;
}

int get_edge(graph *g, size_t s, size_t e)
{
    int *edge;
    return g->v > s && g->v > e && (edge = edge_lookup(g, s, e)) ? edge[g->v] : INT_MAX;
}

int remove_edge(graph *g, size_t s, size_t e)
{
    if (g->v <= s || g->v <= e)
    {
	fprintf(stderr, "no edge in graph\n");
	return EDGE_NOT_FOUND;
    }

    int *needle = edge_lookup(g, s, e);
    if (!needle)
	return EDGE_NOT_FOUND;

    /* сниппет ниже выполняет оптимальное удаление ребра,
       но не сохраняет порядок ребер. */
    
    memcpy(needle, g->mat+(g->edge_current-1)*(g->v+1), sizeof(int)*(g->v+1));
    /* конец сниппета, да */
    g->edge_current--;
    
    return SUCCESS;
}

struct _iterator
{
    int *mat; // матрица инцидентности
    size_t svert; // нач. вершина
    size_t evert; // кон. вершина (задается при вычислении)
    size_t cur; // номер текущего ребра
    size_t vsize; // количество вершин
    size_t esize; // количество ребер
};

size_t get_end(int *edge, size_t l, size_t v)
{
    for (size_t i = 0; i < l; i++)
    {
	if (i != v && edge[i])
	    return i;
    }
    return v;
}

iterator *make_iter(graph *g, size_t v)
{
    if (g->v <= v) return NULL;

    iterator *it = malloc(sizeof(iterator));
    if (!it)
    {
	fprintf(stderr, "could not allocate struct\n");
	return NULL;
    }

    it->mat = g->mat;
    it->svert = v;
    it->cur = 0;
    it->esize = g->edge_current;
    it->vsize = g->v;
    
    if (it->mat[v])
    {
	it->evert = get_end(it->mat, g->v, v);
	return it;
    }
    else
    {
	return it_next(it);
    }
}

iterator *it_next(iterator *it)
{
    if (!it) return NULL;

    for (size_t i = it->cur+1; i < it->esize; i++)
    {
	if (it->mat[i * (it->vsize+1) + it->svert] == 1)
	{
	    it->cur = i;
	    it->evert = get_end(it->mat + i*(it->vsize+1), it->vsize, it->svert);
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
	return it->vsize;
    }
    return it->svert;
}

size_t it_end(iterator *it)
{
    if (!it)
    {
	fprintf(stderr, "iterator exhausted\n");
	return it->vsize;
    }
    return it->evert;
}

int it_data(iterator *it)
{
    if (!it)
    {
	fprintf(stderr, "iterator exhausted\n");
	return INT_MAX;
    }

    return it->mat[it->cur*(it->vsize+1)+it->vsize];
}
