#include "graph.h"
#include "iterator.h"
#include "iograph.h"

#include <stdio.h>
#include <stdlib.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

typedef struct
{
    size_t v;
    size_t w;
} edge;
    
typedef struct _node
{
    edge data;
    struct _node *next;
} node;

typedef struct
{
    node *head;
    edge invval;
} stack;

stack *make_stack(graph *g)
{
    stack *ret = malloc(sizeof(stack));
    ret->head = NULL;
    edge inv = {vertices(g), 0};
    ret->invval = inv;
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

void push(stack *s, edge val)
{
    node *new = malloc(sizeof(node));
    new->data = val;
    new->next = s->head;
    s->head = new;    
}

edge peek(stack *s)
{
    if (!s->head)
    {
	fprintf(stderr, "stack underflow\n");
	return s->invval;
    }
    return s->head->data;
}

edge pop(stack *s)
{
    edge ret = peek(s);
    node *f = s->head;
    s->head = s->head ? s->head->next : NULL;
    free(f);
    return ret;
}

int sempty(stack *s)
{
    return s->head == NULL;
}

void process_block(size_t s, size_t e, stack *stk, size_t *k, size_t s_ch, size_t *num)
{
    ++(*k);
    printf("found block with edges:\n");
    size_t sc = s, ec = e;
    size_t edge_count = 0;
    do
    {
	edge_count++;
        edge block_edge = pop(stk);
	printf("<%lu %lu>\n", block_edge.v, block_edge.w);
	sc = block_edge.v; ec = block_edge.w;
    } while (sc != s && ec != e);

    if (edge_count == 1)
	printf("<%lu %lu> is a cut edge\n", s, e);
    if (num[s] != 1 || s_ch > 1)
	printf("v%lu is a cut vertex\n", s);
    putchar('\n');
}

void block(graph *g, size_t v, size_t p, size_t *num, size_t *low, stack *s, size_t *k)
{
    static size_t n = 0;
    num[v] = low[v] = ++n;
    size_t children = 0;
    edge ins_edge = {v, 0};
    iterator *it = make_iter();
    for (it_init(it, g, v); it_valid(it); it_next(it))
    {
	children++;
	size_t w = it_end(it);
	ins_edge.w = w;
	if (!num[w])
	{
	    push(s, ins_edge);
	    block(g, w, v, num, low, s, k);
	    low[v] = MIN(low[v], low[w]);
	    if (low[w] == num[v])
	    {
	        process_block(v, w, s, k, children, num);
	    }	   
	}
	else
	{
	    low[v] = MIN(low[v], num[w]);
	    if (num[w] < num[v] && w != p)
	    {
		push(s, ins_edge);
	    }
	}	
    }
    free(it);
}

size_t bicon(graph *g)
{
    size_t bcomps = 0;
    stack *s = make_stack(g);

    size_t v = vertices(g);
    size_t *num = calloc(sizeof(size_t), v);
    size_t *low = calloc(sizeof(size_t), v);

    for (size_t i = 0; i < v; i++)
    {
	if (!num[i])
	{
	    block(g, i, i, num, low, s, &bcomps);
	}
    }

    free(num);
    free(low);
    dest_stack(s);
    return bcomps;
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

    size_t bc = bicon(g);
    printf("total bicon comps: %lu\n", bc);
    
    dest_graph(g);
    return 0;
}
