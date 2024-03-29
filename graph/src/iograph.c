#include "graph.h"
#include "iograph.h"
#include "iterator.h"

#include <stdlib.h>
#include <stdio.h>

#define READ_BUF 256

void write_head(FILE *fp, size_t verts, int directed)
{
    fprintf(fp, "%zu %d\n", verts, directed);
}

void write_edge(FILE *fp, size_t start, size_t end, double data)
{
    fprintf(fp, "%zu %zu %g\n", start, end, data);
}

int fwriteg(FILE *fp, graph *g)
{
    if (!fp)
    {
	fprintf(stderr, "file is null\n");
	return FILE_ERROR;
    }

    if (!g)
    {
	fprintf(stderr, "graph is null\n");
	return GRAPH_NULL;
    }

    write_head(fp, vertices(g), directed(g));

    iterator *it = make_iter();
    for (size_t i = 0; i < vertices(g); i++)
    {
	for (it_init(it, g, i); it_valid(it); it_next(it))
	{
	    size_t end = it_end(it);
	    if (!directed(g) && end < i) continue;
	    write_edge(fp, it_start(it), end, it_data(it));
	}
    }
    
    free(it);
    return WRITE_SUCCESS;
}

graph *freadg(FILE *fp)
{
    if (!fp)
    {
	fprintf(stderr, "file is null\n");
	return NULL;
    }

    char buf[READ_BUF];
    if (!fgets(buf, READ_BUF, fp))
    {
	fprintf(stderr, "error while reading file\n");
	return NULL;
    }
    size_t verts;
    int directed;
    // проверка на newline
    char term;
    
    if (sscanf(buf, "%zu %d%c", &verts, &directed, &term) != 3 || term != '\n')
    {
	fprintf(stderr, "wrong head format\n");
	return NULL;
    }
    
    graph *g = make_graph(verts, directed);

    while (fgets(buf, READ_BUF, fp))
    {
	size_t s, e;
	double data;
	if (sscanf(buf, "%zu %zu %lg%c", &s, &e, &data, &term) != 4 || term != '\n')
	{
	    fprintf(stderr, "wrong edge format\n");
	    goto err_cleanup;
	}

	if (s >= verts || e >= verts)
	{
	    fprintf(stderr, "no such vertex in graph\n");
	    goto err_cleanup;
	}

	if (get_edge(g, s, e) != no_edge)
	{
#ifdef STRICT_NONREPEAT
	    fprintf(stderr, "edge duplicate error\n");
	    goto err_cleanup;
#else
	    fprintf(stderr, "warning: edge duplicate\n");
	    continue;
#endif
	}
	add_edge(g, s, e, data);
    }
    if (feof(fp))
	return g;

    if (ferror(fp))
    {
	fprintf(stderr, "error while reading file\n");
    }
err_cleanup:
    dest_graph(g);
    return NULL;
}
