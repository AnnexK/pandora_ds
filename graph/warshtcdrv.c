#include "graph.h"
#include "iterator.h"
#include "iograph.h"

#include <stdlib.h>
#include <stdio.h>

void write_mat(int *mat, size_t s)
{
    for (size_t i = 0; i < s; i++)
    {
	for (size_t j = 0; j < s; j++)
	{
	    printf("%d ", mat[i*s+j] == no_edge ? 0 : 1);
	}
	putchar('\n');
    }
}

graph *warshall_tc(graph *g)
{
    int *mat = make_adj_mat(g);
    size_t v = vertices(g);
    write_mat(mat, v);
    for (size_t i = 0; i < v; i++)
	for (size_t j = 0; j < v; j++)
	    for (size_t k = 0; k < v; k++)
		mat[j*v+k] = (mat[j*v+k] != no_edge) || (mat[j*v+i] != no_edge && mat[i*v+k] != no_edge) ? !no_edge : no_edge;

    graph *ret = make_graph(v, directed(g));
    for (size_t i = 0; i < v; i++)
	for (size_t j = 0; j < v; j++)
	    if (mat[i*v+j] != no_edge)
		add_edge(ret, i, j, mat[i*v+j]);
    free(mat);
    return ret;
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
    
    /* дрижер тута */
    graph *tc = warshall_tc(g);
    writeg(tc);
    
    dest_graph(tc);
    dest_graph(g);

    return 0;
}
