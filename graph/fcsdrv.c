#include "graph.h"
#include "iterator.h"
#include "iograph.h"

#include <stdio.h>
#include <stdlib.h>

void cycle(graph *g, size_t v, size_t *num, size_t *m, size_t *s, size_t *i)
{
    m[v] = ++(*num);
    s[(*i)++] = v;
    
    iterator *it = make_iter();
    for (it_init(it, g, v); it_valid(it); it_next(it))
    {
	size_t end = it_end(it);
	if (!m[end])
	{
	    cycle(g, end, num, m, s, i);
	}
	else if ( m[end] < m[v] && end != s[*i-2] )
	{
	    size_t j;
	    printf("cycle found:\n%zu ", end);
	    for (j = *i-1; s[j] != end; j--)
	    {
		printf("%zu ", s[j]);
	    }
	    printf("%zu\n", s[j]);
	}
    }
    (*i)--;
    free(it);
}

void fcs(graph *g)
{
    size_t *mark = calloc(sizeof(size_t), vertices(g));
    /* стекоподобный массив */
    size_t *s = calloc(sizeof(size_t), vertices(g));
    /* и его индекс */
    size_t s_i = 0;
    size_t num = 0;

    for (size_t i = 0; i < vertices(g); i++)
    {
	if (!mark[i])
	{
	    cycle(g, i, &num, mark, s, &s_i);
	}
    }

    free(s);
    free(mark);
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

    fcs(g);
    
    dest_graph(g);
    return 0;
}
