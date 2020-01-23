#include "graph.h"
#include "iterator.h"

#include <stdlib.h>
#include <stdio.h>

size_t *dfs(graph *g, size_t v)
{
    if (vertices(g) <= v)
    {
	fprintf(stderr, "no vertex in graph\n");
	return NULL;
    }
    
    size_t *ret = malloc(sizeof(size_t) * (vertices(g)+1));
    size_t *visited = calloc(sizeof(unsigned char), vertices(g));

    size_t *last_vert = _dfs(g, v, visited, ret)+1;
    *last_vert = vertices(g);
    
    free(visited);
    return ret;
}

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

int main(void)
{
    return 0;
}
