#include "graph.h"
#include "iterator.h"
#include "iograph.h"

#include <stdio.h>
#include <stdlib.h>

int _topsort(graph *g, size_t v, unsigned char *s, size_t *n, size_t *k)
{
    iterator *it = make_iter();
    s[v] = 1;
    for (it_init(it, g, v); it_valid(it); it_next(it))
    {
	size_t w = it_end(it);
	/* !s[w] -- вершина не посещена (запустить обход из нее),
	   s[w] & !n[w] -- вершина посещена, но не присвоен номер
	   (цикл),
	   s[w] & n[w] -- вершина посещена и присвоен номер
	   (ничего не делать) */
	if ((!s[w] && _topsort(g, w, s, n, k)) || !n[w])
	{
	    free(it); // это *важно*
	    return 1;
	}
    }
    (*k)--;
    n[v] = *k;
    free(it);
    return 0;
}

size_t *topsort(graph *g)
{
    if (!directed(g))
    {
	fprintf(stderr, "topsort does not work with undirected graphs\n");
	return NULL;
    }
    
    unsigned char *state = calloc(vertices(g), sizeof(unsigned char));
    size_t *num = calloc(vertices(g), sizeof(size_t));

    size_t k = vertices(g)+1;

    for (size_t i = 0; i < vertices(g); i++)
    {
	if (!state[i])
	{
	    if (_topsort(g, i, state, num, &k))
	    {
		fprintf(stderr, "cycle found\n");
		free(num);
		free(state);
		return NULL;
	    }
	}	
    }
    free(state);
    return num;
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

    size_t *sorted = topsort(g);
    if (sorted)
    {
	for (size_t i = 0; i < vertices(g); i++)
	{
	    printf("v%zu is #%zu\n", i, sorted[i]);
	}
    }

    dest_graph(g);
    free(sorted);
    return 0;
}
