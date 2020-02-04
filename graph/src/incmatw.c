#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include <string.h>

#include "graph.h"
#include "iterator.h"

#define ALLOC_SIZE 100
/* Как ребро выглядит в памяти:

   |v0|v1|v2|v3|v4|...|vn|edge_weight|
   |1b|1b|1b|1b|1b|...|1b|8 bytes    |
   n -- количество вершин в графе
*/

/* V по unsigned char + double для хранения веса */
#define EDGE_SIZE(V) ((V)*sizeof(unsigned char)+sizeof(double))

struct _graph
{
    unsigned char *mat;
    size_t v;
    size_t edge_capacity;
    size_t edge_current;
    unsigned char directed;
};
const double no_edge = INFINITY;

/* утилити функции для работы с матрицей */
unsigned char *get_mat_edge(unsigned char *mat, size_t v, size_t index)
{
    return (unsigned char *)((char *)mat+index*EDGE_SIZE(v));
}

double *get_mat_edge_data(unsigned char *edge, size_t v)
{
    return (double *)((char *)edge+v*sizeof(unsigned char));
}

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


graph *copy_graph(graph *g)
{
    if (!g) return NULL;
    graph *ret = malloc(sizeof(graph));
    if (!ret)
    {
	fprintf(stderr, "could not create graph struct\n");
	return NULL;
    }

    size_t matsize = g->edge_capacity*(EDGE_SIZE(g->v));
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

unsigned char directed(graph *g)
{
    return g->directed;
}

unsigned char *edge_lookup(graph *g, size_t s, size_t e)
{
    int type = g->directed && s != e ? -1 : 1;
    for (size_t i = 0; i < g->edge_current; i++)
    {
	unsigned char *edge = get_mat_edge(g->mat, g->v, i);
	if (edge[s] && edge[e] == type)
	    return edge;
    }
    return NULL;
}

// data is weight
int add_edge(graph *g, size_t s, size_t e, double data)
{
    if (g->v <= s || g->v <= e)
    {
	fprintf(stderr, "no edge in graph\n");
	return EDGE_NOT_FOUND;
    }
    unsigned char *edge;
    if ((edge = edge_lookup(g, s, e)) == NULL)
    {
	if (g->edge_capacity == g->edge_current)
	{
	    size_t new_size = (g->edge_current+ALLOC_SIZE)*EDGE_SIZE(g->v);
	    unsigned char *tmp = realloc(g->mat, new_size);
	    if (!tmp)
	    {
		fprintf(stderr, "could not reallocate incidence matrix\n");
		return ALLOC_ERROR;
	    }
	    
	    g->mat = tmp;
	    g->edge_capacity += ALLOC_SIZE;
	}
        edge = get_mat_edge(g->mat, g->v, g->edge_current);
	
	for (size_t i = 0; i < g->v; i++)
	{
	    edge[i] = 0;
	}
    
	// s == e -- петля, поэтому на e пишем 1
	edge[e] = g->directed ? -1 : 1;
	edge[s] = 1;
    }

    /* пишем в ребро его вес */
    *get_mat_edge_data(edge, g->v) = data;

    g->edge_current++;
    return SUCCESS;
}

double get_edge(graph *g, size_t s, size_t e)
{
    unsigned char *edge = edge_lookup(g, s, e);
    return g->v > s && g->v > e && edge ? *get_mat_edge_data(edge, g->v) : no_edge;
}

int remove_edge(graph *g, size_t s, size_t e)
{
    if (g->v <= s || g->v <= e)
    {
	fprintf(stderr, "no edge in graph\n");
	return EDGE_NOT_FOUND;
    }

    unsigned char *needle = edge_lookup(g, s, e);
    if (!needle)
	return EDGE_NOT_FOUND;

    /* сниппет ниже выполняет оптимальное удаление ребра,
       но не сохраняет порядок ребер. */

    unsigned char *last_edge = get_mat_edge(g->mat, g->v, g->edge_current-1);
    memcpy(needle, last_edge, EDGE_SIZE(g->v));
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
	    unsigned char *edge = get_mat_edge(g->mat, g->v, e);
	    for (size_t i = 0; i < v; i++)
	    {
		edge[i] *= -1;
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
	unsigned char *edge = get_mat_edge(g->mat, g->v, i);
	size_t s = v;
	size_t e = v;
	for (size_t j = 0; j < v; j++)
	{
	    if (edge[j] == -1)
		e = j;
	    else if (edge[j] == 1)
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
	double d = *get_mat_edge_data(edge, v);
	ret[s*v+e] = d;
	if (!directed(g))
	    ret[e*v+s] = d;
    }
    return ret;
}

struct _iterator
{
    unsigned char *mat; // матрица инцидентности
    size_t svert; // нач. вершина
    size_t evert; // кон. вершина (задается при вычислении)
    size_t cur; // номер текущего ребра
    size_t vsize; // количество вершин
    size_t esize; // количество ребер
};

iterator *make_iter(void)
{
    return malloc(sizeof(iterator));
}

size_t get_end(unsigned char *edge, size_t l, size_t v)
{
    for (size_t i = 0; i < l; i++)
    {
	if (i != v && edge[i])
	    return i;
    }
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

    unsigned char *edge;
    for (it->cur++; it->cur < it->esize; it->cur++)
    {
	edge = get_mat_edge(it->mat, it->vsize, it->cur);
	if (edge[it->svert] == 1)
	{
	    it->evert = get_end(edge, it->vsize, it->svert);
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

    unsigned char *edge = get_mat_edge(it->mat, it->vsize, it->cur);
    return *get_mat_edge_data(edge, it->vsize);
}

int it_valid(iterator *it)
{
    return it && it->cur < it->esize;
}
