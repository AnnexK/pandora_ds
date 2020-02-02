#include "graph.h"
#include "iterator.h"
#include "iograph.h"
#include "list.h"


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct
{
    size_t *repr;
    size_t *size;
} dsu;


dsu *make_dsu(size_t size)
{
    dsu *ret = malloc(sizeof(dsu));
    ret->repr = malloc(size * sizeof(size_t));
    ret->size = malloc(size * sizeof(size_t));

    for (size_t i = 0; i < size; ++i) // изначально каждый элемент содержится в своем множестве
    {
        ret->repr[i] = i;
        ret->size[i] = 1;        
    }

    return ret;    
}

void dest_dsu(dsu *ptr)
{
    free(ptr->repr);
    free(ptr->size);
    free(ptr);
}

size_t find(dsu *d, size_t x)
{
    if (d->repr[x] == x) // найден представитель множества
        return x;
    return d->repr[x] = find(d, d->repr[x]);    
}

void dmerge(dsu *d, size_t a, size_t b)
{
    size_t repr_a = find(d, a), repr_b = find(d, b);
    if (repr_a == repr_b) // элементы уже в одном множестве
        return;
    if (d->size[repr_a] < d->size[repr_b])
    {
        d->repr[repr_a] = repr_b;
        d->size[repr_b] += d->size[repr_a];
    }
    else
    {
        d->repr[repr_b] = repr_a;
        d->size[repr_a] += d->size[repr_b];
    }
}

int same(dsu *d, size_t a, size_t b)
{
    return find(d, a) == find(d, b); // одинаковый представитель -> одинаковое множество
}

typedef struct
{
    size_t s;
    size_t e;
    int w;
} edge;

int edgecmp(const void *a, const void *b)
{
    int edge_a = ((edge *)a)->w, edge_b = ((edge *)b)->w;
    if (edge_a < edge_b)
	return -1;
    else if (edge_a == edge_b)
	return 0;
    else return 1;
}

list *get_edges(graph *g)
{
    list *l = make_list(sizeof(edge), edgecmp);
    iterator *it = make_iter();

    for (size_t i = 0; i < vertices(g); i++)
    {
	for (it_init(it, g, i); it_valid(it); it_next(it))
	{
	    edge ins = {it_start(it), it_end(it), it_data(it)};
	    insert(l, NULL, &ins);
	}
    }

    free(it);
    return l;
}

graph *kruskal(graph *g)
{
    if (directed(g))
    {
	fprintf(stderr, "graph is directed\n");
	return NULL;
    }
    
    size_t v = vertices(g);
    graph *mst = make_graph(v, 0);

    list *edges = get_edges(g);
    lsort(edges, edgecmp);

    dsu *d = make_dsu(v);
    size_t nedges = 0;
    for (node *n = first(edges); n && nedges < v-1; n = next(n))
    {
	edge *ed = data(n);
	if (!same(d, ed->s, ed->e))
	{
	    add_edge(mst, ed->s, ed->e, ed->w);
	    dmerge(d, ed->s, ed->e);
	    nedges++;
	}
    }
    if (nedges < v-1)
    {
	fprintf(stderr, "could not build an MST\n");
	dest_graph(mst);
	mst = NULL;
    }

    dest_dsu(d);
    dest_list(edges);
    return mst;
}
int main(int argc, char **argv)
{
    if (argc != 2)
    {
	fprintf(stderr, "usage: %s filename\n", *argv);
	return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp)
    {
	fprintf(stderr, "%s: error: could not open file\n", *argv);
	return 1;
    }
    graph *g = freadg(fp);
    if (!g)
    {
	fprintf(stderr, "%s: error: could not read graph\n", *argv);
	return 1;
    }
    fclose(fp);

    graph *mst = kruskal(g);
    if (mst)
    {
	writeg(mst);
	iterator *it = make_iter();
	int sum = 0;
	for (size_t i = 0; i < vertices(mst); i++)
	    for (it_init(it, mst, i); it_valid(it); it_next(it))
		if (it_end(it) < i)
		    sum += it_data(it);
	printf("MST weight sum = %d\n", sum);
	dest_graph(mst);
	free(it);
    }
    dest_graph(g);
    return 0;
}
