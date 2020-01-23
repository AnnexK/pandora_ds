#include <stdlib.h>
#include <stdio.h>

#include "graph.h"

struct _graph
{
    int *mat;
};

graph *make_graph(size_t v_amt)
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

    return g;
}
