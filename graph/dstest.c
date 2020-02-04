#include "graph.h"
#include "iograph.h"
#include "iterator.h"

#include <stdio.h>
#include <stdlib.h>

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
	return 1;
    FILE *fp = fopen(argv[1], "r");
    if (!fp)
	return 1;

    graph *g = freadg(fp);
    if (!g)
	return 1;
    fclose(fp);

    writeg(g);
    
    double *mat = make_adj_mat(g);
    print_mat(mat, vertices(g), 8);
    free(mat);

    dest_graph(g);
    return 0;
}
