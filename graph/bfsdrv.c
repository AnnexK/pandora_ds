#include "graph.h"
#include "iterator.h"
#include "iograph.h"

#include <stdlib.h>
#include <stdio.h>

typedef struct
{
    size_t *arr;
    size_t head;
    size_t tail;
    size_t arr_size;
    size_t invval;
} queue;

queue *make_queue(graph *g)
{
    queue *q = malloc(sizeof(queue));
    q->head = q->tail = 0;
    q->arr_size = vertices(g)+1;
    q->arr = malloc(q->arr_size*sizeof(size_t));
    q->invval = vertices(g);
    return q;
}

void dest_queue(queue *q)
{
    free(q->arr);
    free(q);
}

void enqueue(queue *q, size_t val)
{
    if ( (q->tail+1)%q->arr_size == q->head )
    {
	fprintf(stderr, "Queue overflow\n");
	return;
    }
    q->arr[q->tail] = val;
    q->tail = (q->tail+1) % q->arr_size;
}

size_t dequeue(queue *q)
{
    if ( q->head == q->tail )
    {
	fprintf(stderr, "Queue underflow\n");
	return q->invval;
    }
    size_t ret = q->arr[q->head];
    q->head = (q->head+1) % q->arr_size;
    return ret;
}

int qempty(queue *q)
{
    return q->head == q->tail;
}

void bfs(graph *g, size_t v)
{
    iterator *it = make_iter();
    if (vertices(g) <= v)
    {
	fprintf(stderr, "no vertex in graph\n");
	return;
    }

    size_t *visited = calloc(sizeof(size_t), vertices(g));
    visited[v] = 1;
    
    queue *q = make_queue(g);
    enqueue(q, v);

    while (!qempty(q))
    {
	size_t w = dequeue(q);
	/* вывод вершины */
        printf("%lu ", w);
	for (it_init(it, g, w); it_valid(it); it_next(it))
	{
	    size_t y = it_end(it);
	    if (!visited[y])
	    {
		visited[y] = 1;
		enqueue(q, y);
	    }
	}
    }
    dest_queue(q);
    free(visited);
    putchar('\n');
    free(it);
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
    
    bfs(g, 0);

    dest_graph(g);

    return 0;
}
