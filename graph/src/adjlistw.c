#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <math.h>

#include "graph.h"
#include "iterator.h"
#include "list.h"

// list of struct _listdata
struct _graph
{
    list **adjlists;
    size_t v;
    int directed;
};
const double no_edge = INFINITY;

typedef struct _listdata
{
    size_t v;
    double weight;
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

graph *make_graph(size_t v_amt, int dir)
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

int directed(graph *g)
{
    return g->directed;
}

// data is weight
int add_edge(graph *g, size_t s, size_t e, double d)
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
	if (!g->directed)
	{
	    ((listdata *)data(search(g->adjlists[e], &s)))->weight = d;
	}
    }
    else
    {
	listdata key = {e, d};
	insert(g->adjlists[s], last(g->adjlists[s]), &key);
	if (!g->directed)
	{
	    key.v = s;
	    insert(g->adjlists[e], last(g->adjlists[e]), &key);
	}
    }
    return SUCCESS;
}

double get_edge(graph *g, size_t s, size_t e)
{
    node *edge;
    return g->v > s && g->v > e && (edge = search(g->adjlists[s], &e)) ? ((listdata *)data(edge))->weight : no_edge;
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

void move_next(graph *g, size_t v)
{
    listdata *tmp = data(first(g->adjlists[v]));
    size_t new_end = tmp->v;
    tmp->v = v;
    insert(g->adjlists[new_end], last(g->adjlists[new_end]), tmp);
    delete(g->adjlists[v], first(g->adjlists[v]));
}

/* ToDo: добавить операцию append(L1, L2) в список
   и переписать это */
void transpose(graph *g)
{
    node **ends = malloc(vertices(g) * sizeof(node *));
    for (size_t i = 0; i < vertices(g); i++)
    {
	ends[i] = last(g->adjlists[i]);
    }

    for (size_t i = 0; i < vertices(g); i++)
    {
	if (ends[i]) // список i был не пуст до вставок
	{
	    while (first(g->adjlists[i]) != ends[i])
	    {
		move_next(g, i);
	    }
	    move_next(g, i);
	}
    }
    free(ends);
}

double *make_adj_mat(graph *g)
{
    size_t v = vertices(g);
    double *ret = malloc(sizeof(double) * v * v);

    for (size_t i = 0; i < v * v; i++)
    {
	ret[i] = no_edge;
    }

    for (size_t i = 0; i < v; i++)
    {
	for (node *n = first(g->adjlists[i]); n; n = next(n))
	{
	    listdata *d = data(n);
	    ret[i*v+d->v] = d->weight;
	}
    }
    return ret;
}

struct _iterator
{
    size_t size;
    size_t vert;    
    node *cur;
};

iterator *make_iter(void)
{
    return malloc(sizeof(iterator));
}

void set_invalid(iterator *it)
{
    it->cur = NULL;
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
    it->cur = first(g->adjlists[v]);
    it->size = g->v;
}

void it_next(iterator *it)
{
    if (!it_valid(it)) return;
    it->cur = next(it->cur);
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
    listdata *d = data(it->cur);
    return d->v;
}

double it_data(iterator *it)
{
    if (!it_valid(it))
    {
	fprintf(stderr, "iterator exhausted\n");
	return no_edge;
    }
    return ((listdata *)data(it->cur))->weight;
}

int it_valid(iterator *it)
{
    return it && it->cur != NULL;
}
