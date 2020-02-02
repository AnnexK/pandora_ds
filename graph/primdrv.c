#include "graph.h"
#include "iterator.h"
#include "iograph.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

graph *prim(graph *g)
{
    if (directed(g))
    {
	fprintf(stderr, "graph is directed\n");
	return NULL;
    }
    
    size_t v = vertices(g);
    iterator *it = make_iter();
    
    graph *mst = make_graph(v, 0);
    unsigned char *vec = calloc(sizeof(unsigned char), v);
    vec[0] = 1;
    size_t in_graph = 1;

    while (in_graph < v)
    {
	size_t s = v, e = v;
	int min = INT_MAX;
	for (size_t i = 0; i < v; i++)
	{
	    if (vec[i])
	    {	   
		for (it_init(it, g, i); it_valid(it); it_next(it))
		{
		    size_t j = it_end(it);
		    if (!vec[j] && it_data(it) < min)
		    {
			s = i;
			e = j;
			min = it_data(it);
		    }
		}
	    }
	}
	if (s == v)
	{
	    break;
	}
	else
	{
	    add_edge(mst, s, e, min);
	    vec[s] = 1;
	    vec[e] = 1;
	    in_graph++;
	}   
    }

    if (in_graph < v)
    {
	fprintf(stderr, "could not build a spanning tree\n");
	dest_graph(mst);
	mst = NULL;
    }
    free(vec);
    free(it);
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

    graph *mst = prim(g);
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
