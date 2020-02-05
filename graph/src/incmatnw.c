#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include <string.h>

#include "graph.h"
#include "iterator.h"

#define ALLOC_SIZE 100

struct _graph
{
    int *mat;
    size_t v;
    size_t edge_capacity;
    size_t edge_current;
    int directed;
};
const double no_edge = INFINITY;

graph *make_graph(size_t v_amt, int dir)
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

graph *copy_graph(graph *g)
{
    if (!g) return NULL;
    graph *ret = malloc(sizeof(graph));
    if (!ret)
    {
	fprintf(stderr, "could not create graph struct\n");
	return NULL;
    }

    size_t matsize = g->edge_capacity*g->v*sizeof(int);
    ret->mat = (g->mat ? malloc(matsize) : NULL);
    if (g->mat && !ret->mat)
    {
	fprintf(stderr, "could not allocate matrix\n");
	free(ret);
	return NULL;
    }
    
    ret->v = g->v;
    ret->directed = g->directed;
    ret->edge_capacity = g->edge_capacity;
    ret->edge_current = g->edge_current;

    if (g->mat) memcpy(ret->mat, g->mat, matsize);
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

int directed(graph *g)
{
    return g->directed;
}

int *edge_lookup(graph *g, size_t s, size_t e)
{
    int type = g->directed && s != e ? -1 : 1;
    for (size_t i = 0; i < g->edge_current; i++)
    {
	if (g->mat[i*(g->v)+s] && g->mat[i*(g->v)+e] == type)
	    return g->mat+i*(g->v);
    }
    return NULL;
}

// data is ignored
int add_edge(graph *g, size_t s, size_t e, double data)
{
    if (g->v <= s || g->v <= e)
    {
	fprintf(stderr, "no edge in graph\n");
	return EDGE_NOT_FOUND;
    }

    if (!edge_lookup(g, s, e))
    {
	if (g->edge_capacity == g->edge_current)
	{
	    int *tmp = realloc(g->mat, (g->edge_current+ALLOC_SIZE)*(g->v)*sizeof(int));
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
	    g->mat[g->edge_current*(g->v)+i] = 0;
	}

	// если s == е, то будет одна единица
	g->mat[g->edge_current*(g->v)+e] = g->directed ? -1 : 1;
	g->mat[g->edge_current*(g->v)+s] = 1;

	g->edge_current++;
    }
    return SUCCESS;
}

double get_edge(graph *g, size_t s, size_t e)
{
    return g->v > s && g->v > e && edge_lookup(g, s, e) ? 1.0 : no_edge;
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
    
    memcpy(needle, g->mat+(g->edge_current-1)*(g->v), sizeof(int)*(g->v));
    /* конец сниппета, да */
    g->edge_current--;
    
    return SUCCESS;
}

void transpose(graph *g)
{
    if (directed(g))
    {
	size_t v = vertices(g);
	for (size_t e = 0; e < g->edge_current; e++)
	{
	    for (size_t i = 0; i < v; i++)
	    {
		g->mat[e*v+i] *= -1;
	    }
	}
    }
}

double *make_adj_mat(graph *g)
{
    size_t v = vertices(g);
    double *ret = malloc(sizeof(double) * v * v);
    for (size_t i = 0; i < v * v; i++)
    {
	ret[i] = no_edge;
    }
    for (size_t i = 0; i < g->edge_current; i++)
    {
	size_t s = v;
	size_t e = v;
	for (size_t j = 0; j < v; j++)
	{
	    if (g->mat[i*v+j])
	    {
		if (s == v)
		    s = j;
		else
		    e = j;
	    }
	}
	/* петля */
	if (e == v)
	    e = s;
	ret[s*v+e] = 1.0;
	if (!directed(g))
	    ret[e*v+s] = 1.0;
    }
    return ret;
}

struct _iterator
{
    int *mat; // матрица инцидентности
    size_t svert; // нач. вершина
    size_t evert; // кон. вершина (задается при вычислении it_next)
    size_t cur; // номер текущего ребра
    size_t vsize; // количество вершин
    size_t esize; // количество ребер
};

iterator *make_iter(void)
{
    return malloc(sizeof(iterator));
}

size_t get_end(int *edge, size_t l, size_t v)
{
    for (size_t i = 0; i < l; i++)
    {
	if (i != v && edge[i])
	    return i;
    }
    // ребро есть петля -- возвращаем саму вершину
    return v;
}

void set_invalid(iterator *it)
{
    it->cur = it->esize = 0;
}

void it_init(iterator *it, graph *g, size_t v)
{
    if (!it) return;
    if (g->v <= v)
    {
	set_invalid(it);
	return;
    }

    it->mat = g->mat;
    it->svert = v;
    it->cur = 0;
    it->esize = g->edge_current;
    it->vsize = g->v;
    
    if (it->mat[v] == 1)
    {
	it->evert = get_end(it->mat, g->v, v);
    }
    else
    {
	it_next(it);
    }
}

void it_next(iterator *it)
{
    if (!it_valid(it)) return;

    for (it->cur++; it->cur < it->esize; it->cur++)
    {
	if (it->mat[it->cur * (it->vsize) + it->svert] == 1)
	{
	    it->evert = get_end(it->mat + it->cur*(it->vsize), it->vsize, it->svert);
	    return;
	}
    }
}

size_t it_start(iterator *it)
{
    if (!it_valid(it))
    {
	fprintf(stderr, "iterator exhausted\n");
	return it->vsize;
    }
    return it->svert;
}

size_t it_end(iterator *it)
{
    if (!it_valid(it))
    {
	fprintf(stderr, "iterator exhausted\n");
	return it->vsize;
    }
    return it->evert;
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
    return it && it->cur < it->esize;
}
