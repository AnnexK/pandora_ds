#include "graph.h"
#include "iterator.h"
#include "iograph.h"

#include <stdlib.h>
#include <stdio.h>

void _dfs(graph *g, size_t v, size_t *visited)
{
    visited[v] = 1;
    printf("%zu ", v);
    iterator *it = make_iter();
    for (it_init(it, g, v); it_valid(it); it_next(it))
    {
	size_t end = it_end(it);
	if (!visited[end])
	{
	    _dfs(g, end, visited);
	}
    }
    free(it);
}

void dfs(graph *g, size_t v)
{
    if (vertices(g) <= v)
    {
	fprintf(stderr, "no vertex in graph\n");
	return;
    }
    
    size_t *visited = calloc(sizeof(size_t), vertices(g));

    _dfs(g, v, visited);
    
    free(visited);
    putchar('\n');
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
    
    dfs(g, 0);

    dest_graph(g);

    return 0;
}
