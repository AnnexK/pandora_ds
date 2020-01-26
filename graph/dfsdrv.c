#include "graph.h"
#include "iterator.h"
#include "iograph.h"

#include <stdlib.h>
#include <stdio.h>

size_t *_dfs(graph *g, size_t v, size_t *visited, size_t *ret)
{
    visited[v] = 1;
    *ret = v;

    for (iterator *it = make_iter(g, v); it; it = it_next(it))
    {
	size_t end = it_end(it);
	if (!visited[end])
	{
	    ret = _dfs(g, end, visited, ret+1);
	}
    }
    return ret;
}

size_t *dfs(graph *g, size_t v)
{
    if (vertices(g) <= v)
    {
	fprintf(stderr, "no vertex in graph\n");
	return NULL;
    }
    
    size_t *ret = malloc(sizeof(size_t) * (vertices(g)+1));
    size_t *visited = calloc(sizeof(size_t), vertices(g));

    size_t *last_vert = _dfs(g, v, visited, ret)+1;
    *last_vert = vertices(g);
    
    free(visited);
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
    
    size_t *visited_order = dfs(g, 0);

    size_t i = 0;
    while (visited_order[i] != vertices(g))
    {
	printf("%lu ", visited_order[i++]);
    }
    putchar('\n');
    free(visited_order);
    dest_graph(g);

    return 0;
}
