#include "graph.h"
#include "iterator.h"
#include "iograph.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
    size_t *arr;
    size_t size; // также работает как "неправильное" значение
    size_t cur;
} stack;

stack *make_stack(graph *g)
{
    stack *ret = malloc(sizeof(stack));
    ret->size = vertices(g);
    ret->cur = 0;
    ret->arr = malloc(sizeof(size_t) * ret->size);
    return ret;
}

void dest_stack(stack *s)
{
    free(s->arr);
    free(s);
}

void push(stack *s, size_t val)
{
    if (s->cur == s->size)
    {
	fprintf(stderr, "stack overflow\n");
    }
    else
    {
	s->arr[s->cur++] = val;
    }
}

size_t pop(stack *s)
{
    if (s->cur == 0)
    {
	fprintf(stderr, "stack underflow\n");
	return s->size;
    }

    return s->arr[--s->cur];
}

int sempty(stack *s)
{
    return s->cur == 0;
}


/* Basic Connected Components -- для поиска КС в неорграфе */
void _bcc(graph *g, size_t v, size_t *ccvec, size_t ncc)
{
    ccvec[v] = ncc;
    iterator *it = make_iter();
    for (it_init(it, g, v); it_valid(it); it_next(it))
    {
	size_t end = it_end(it);
	if (!ccvec[end])
	{
	    _bcc(g, end, ccvec, ncc);
	}
    }
    free(it);
}

size_t *bcc(graph *g)
{
    size_t *ret = calloc(sizeof(size_t), vertices(g));
    size_t ncc = 0;

    for (size_t i = 0; i < vertices(g); i++)
    {
	if (!ret[i])
	{
	    ncc++;
	    _bcc(g, i, ret, ncc);
	}
    }
    return ret;
}

/* Strongly Connected Components -- для поиска КСС в орграфе */

void dfs_order(graph *g, size_t v, stack *ord, size_t *visited)
{
    visited[v] = 1;
    iterator *it = make_iter();
    for (it_init(it, g, v); it_valid(it); it_next(it))
    {
	size_t end = it_end(it);
	if (!visited[end])
	{
	    dfs_order(g, end, ord, visited);
	}
    }
    push(ord, v);
    free(it);
}

void dfs_comp(graph *g, size_t v, size_t *ccvec, size_t ncc)
{
    ccvec[v] = ncc;
    iterator *it = make_iter();
    for (it_init(it, g, v); it_valid(it); it_next(it))
    {
	size_t end = it_end(it);
	if (!ccvec[end])
	{
	    dfs_comp(g, end, ccvec, ncc);
	}
    }
    free(it);
}

size_t *scc(graph *g)
{
    size_t *ret = calloc(sizeof(size_t), vertices(g));
    stack *ord = make_stack(g);
    
    for (size_t i = 0; i < vertices(g); i++)
    {
	if (!ret[i])
	{
	    dfs_order(g, i, ord, ret);
	}
    }

    memset(ret, 0, sizeof(size_t) * vertices(g));
    
    size_t ncc = 0;
    transpose(g);

    while (!sempty(ord))
    {
	size_t i = pop(ord);
	if (!ret[i])
	{
	    ncc++;
	    dfs_comp(g, i, ret, ncc);
	}
    }
    transpose(g);
    dest_stack(ord);
    return ret;
}

size_t *cc(graph *g)
{
    return directed(g) ? scc(g) : bcc(g);
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

    size_t *compv = cc(g);
    for (size_t i = 0; i < vertices(g); i++)
    {
	printf("v%zu is in cc%zu\n", i, compv[i]);
    }
    free(compv);

    dest_graph(g);

    return 0;
}
