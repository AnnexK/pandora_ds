#include "graph.h"
#include "iterator.h"
#include "iograph.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MIN(A, B) ((A) < (B) ? (A) : (B))

double *floyd(graph *g)
{
    double *mat = make_adj_mat(g);
    size_t v = vertices(g);
    /* обработка наличия/отсутствия петель */
    for (size_t i = 0; i < v; i++)
	mat[i*v+i] = 0.0;
    
    for (size_t k = 0; k < v; k++)
	for (size_t i = 0; i < v; i++)
	    for (size_t j = 0; j < v; j++)
		mat[i*v+j] = MIN(mat[i*v+j], mat[i*v+k]+mat[k*v+j]);
    return mat;
}

void print_mat(double *mat, size_t d, int width)
{
    for (size_t i = 0; i < d; i++)
    {
	for (size_t j = 0; j < d; j++)
	{
	    printf("%*g", width, mat[i*d+j]);
	}
	putchar('\n');
    }	    
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

    double *apsp_mat = floyd(g);
    print_mat(apsp_mat, vertices(g), 8);
    
    free(apsp_mat);
    dest_graph(g);
    return 0;
}
