#include "graph.h"
#include "iterator.h"
#include "iograph.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

size_t *topsort(graph *g)
{
    size_t v = vertices(g);
    /* в ret хранится отображение порядка вершин в ТС в множество вершин,
       т. е. ret[i] == v означает, что вершина v является i-ой в ТС */
    size_t *ret = malloc(sizeof(size_t) * v);
    // предки каждой вершины
    size_t *p = calloc(sizeof(size_t), v);
    
    size_t *stk = malloc(sizeof(size_t)*v);
    size_t stkp = 0;
    
    iterator *it = make_iter();

    /* подсчет предков */
    for (size_t i = 0; i < v; i++)
	for (it_init(it, g, i); it_valid(it); it_next(it))
	    p[it_end(it)]++;

    for (size_t i = 0; i < v; i++)
	if (!p[i]) // нет предков -- первая вершина
	    stk[stkp++] = i;
    
    size_t num = 0;
    while (stkp)
    {
	size_t w = stk[--stkp];
	ret[num++] = w;
	for (it_init(it, g, w); it_valid(it); it_next(it))
	{
	    size_t v = it_end(it);
	    p[v]--;
	    if (!p[v])
		stk[stkp++] = v;
	}
    }

    if (num < v)
    {
	fprintf(stderr, "graph contains cycles\n");
	free(ret);
	ret = NULL;
    }
    free(stk);
    free(it);
    free(p);
    return ret;
}

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

typedef struct
{
    size_t *p;
    double *d;
} result;

result dagsp(graph *g, size_t s)
{
    size_t *order = topsort(g);
    size_t *p;
    double *dists;
    if (!order)
    {
	fprintf(stderr, "could not find paths\n");
	p = NULL;
	dists = NULL;
    }
    else
    {
	size_t v = vertices(g);
	init_ss(v, s, &p, &dists);
	iterator *it = make_iter();
	size_t i;
	/* пропускаем вершины, которые находятся раньше s в ТС */
	for (i = 0; order[i] != s; i++);
	/* и обрабатываем остальные вершины */
	for (; i < v; i++)
	    for (it_init(it, g, order[i]); it_valid(it); it_next(it))
		relax(it, p, dists);
	free(it);
    }

    free(order);
    result res = {p, dists};
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
    printf("Choose starting vertex from 0 to %lu: ", vertices(g)-1);
    char buf[256];
    do
    {
	fgets(buf, 256, stdin);
	sscanf(buf, "%lu", &s);
    } while (s >= vertices(g) && printf("try again\n"));
    
    result res = dagsp(g, s);
    if (res.d)
    {
	size_t *stk = malloc(sizeof(size_t) * vertices(g));
	size_t stkptr = 0;
	for (size_t i = 0; i < vertices(g); i++)
	{
	    if (i != s && res.d[i] < INFINITY)
	    {       
		printf("distance from v%lu to v%lu equals %g\n", s, i, res.d[i]);
		for (size_t cur = i; cur != s; cur = res.p[cur])
		    stk[stkptr++] = cur;
		printf("path: %lu ", s);
		while (stkptr)
		    printf("%lu ", stk[--stkptr]);
		putchar('\n');
	    }
	}
	
	free(res.d);
	free(res.p);
	free(stk);
    }

    dest_graph(g);
    return 0;
}
