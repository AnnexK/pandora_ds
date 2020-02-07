#include "graph.h"
#include "iterator.h"
#include "iograph.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void init_ss(size_t v, size_t s, size_t **p, double **d)
{
    *p = malloc(sizeof(size_t) * v);
    *d = malloc(sizeof(double) * v);
    for (size_t i = 0; i < v; i++)
    {
	(*p)[i] = v;
	(*d)[i] = INFINITY;
    }
    (*d)[s] = 0;
    (*p)[s] = s;
}

void relax(iterator *it, size_t *p, double *d)
{
    size_t s = it_start(it), e = it_end(it);
    double edgw = it_data(it);
    if (d[e] > d[s] + edgw)
    {
	d[e] = d[s] + edgw;
	p[e] = s;
    }
}

size_t select_min(double *dists, unsigned char *fixed, size_t v)
{
    size_t min = v;
    for (size_t i = 0; i < v; i++)
	if ((min == v || dists[i] < dists[min]) && !fixed[i])
	    min = i;
    return min;
}

typedef struct
{
    size_t *p;
    double *d;
} result;

result dijkstra(graph *g, size_t start)
{
    size_t v = vertices(g);
    size_t *parents;
    double *dists;
    unsigned char *fixed = calloc(sizeof(unsigned char), v);
    
    init_ss(v, start, &parents, &dists);
    
    size_t cur;
    iterator *it = make_iter();
    while ((cur = select_min(dists, fixed, v)) != v && dists[cur] < INFINITY)
    {
	fixed[cur] = 1;
	for (it_init(it, g, cur); it_valid(it); it_next(it))
	{
	    relax(it, parents, dists);
	}
    }
    free(it);
    free(fixed);
    result res = {parents, dists};
    return res;
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

    size_t s = vertices(g);
    printf("Choose starting vertex from 0 to %zu: ", vertices(g)-1);
    char buf[256];
    do
    {
	fgets(buf, 256, stdin);
	sscanf(buf, "%zu", &s);
    } while (s >= vertices(g) && printf("try again\n"));
    
    result res = dijkstra(g, s);
    
    size_t *stk = malloc(sizeof(size_t) * vertices(g));
    size_t stkptr = 0;
    for (size_t i = 0; i < vertices(g); i++)
    {
	if (i != s && res.d[i] < INFINITY)
	{       
	    printf("distance from v%zu to v%zu equals %g\n", s, i, res.d[i]);
	    for (size_t cur = i; cur != s; cur = res.p[cur])
		stk[stkptr++] = cur;
	    printf("path: %zu ", s);
	    while (stkptr)
		printf("%zu ", stk[--stkptr]);
	    putchar('\n');
	}
    }

    free(res.d);
    free(res.p);
    free(stk);
    dest_graph(g);

    return 0;
}
