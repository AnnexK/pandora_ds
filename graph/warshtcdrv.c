#include "graph.h"
#include "iterator.h"
#include "iograph.h"

#include <stdlib.h>
#include <stdio.h>

void write_mat(const int *mat, size_t s)
{
    for (size_t i = 0; i < s; i++)
    {
	for (size_t j = 0; j < s; j++)
	{
	    printf("%d ", mat[i*s+j]);
	}
	putchar('\n');
    }
}

/* определено *ТОЛЬКО* в src/adjmatnw.c
   остальные СД не слинкуются
   модифицирует исходный граф */
const int *warshall_mat(graph *);

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

    // возвращает указатель на матрицу *внутри* СД
    const int *tc = warshall_mat(g);
    write_mat(tc, vertices(g));
    dest_graph(g);

    return 0;
}
