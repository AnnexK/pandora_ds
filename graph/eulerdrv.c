#include "graph.h"
#include "iterator.h"
#include "iograph.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct _node
{
    size_t data;
    struct _node *next;
} node;

typedef struct
{
    node *head;
    size_t invval;
} stack;

stack *make_stack(graph *g)
{
    stack *ret = malloc(sizeof(stack));
    ret->head = NULL;
    ret->invval = vertices(g);
    return ret;
}

void dest_stack(stack *s)
{
    node *cur = s->head;
    while (cur)
    {
	node *f = cur;
	cur = cur->next;
	free(f);
    }
    free(s);
}

void push(stack *s, size_t val)
{
    node *new = malloc(sizeof(node));
    new->data = val;
    new->next = s->head;
    s->head = new;    
}

size_t peek(stack *s)
{
    if (!s->head)
    {
	fprintf(stderr, "stack underflow\n");
	return s->invval;
    }
    return s->head->data;
}

size_t pop(stack *s)
{
    size_t ret = peek(s);
    node *f = s->head;
    s->head = s->head ? s->head->next : NULL;
    free(f);
    return ret;
}

int sempty(stack *s)
{
    return s->head == NULL;
}

stack *euler(graph *g)
{
    if (vertices(g) == 0)
    {
	fprintf(stdout, "graph is empty\n");
	return make_stack(g);
    }

    iterator *it = make_iter();
    /* мб добавить проверку на наличие */
    stack *s1 = make_stack(g);
    stack *s2 = make_stack(g);

    push(s1, 0);

    while (!sempty(s1))
    {
	size_t v = peek(s1);
	/* проверка на непустоту множества смежных с v вершин */
	it_init(it, g, v);
	if (it_valid(it))
	{
	    size_t w = it_end(it);
	    push(s1, w);
	    remove_edge(g, v, w);
	}
	else
	{
	    push(s2, pop(s1));
	}
    }

    free(it);
    dest_stack(s1);
    return s2;
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

    stack *cycle = euler(g);
    while (!sempty(cycle))
    {
	printf("%zu ", pop(cycle));
    }

    putchar('\n');
    dest_stack(cycle);
    dest_graph(g);
    return 0;
}
