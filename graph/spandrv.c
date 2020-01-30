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

void _dfs(graph *g, size_t v, unsigned char *visited, graph *mst)
{
    visited[v] = 1;
    iterator *it = make_iter();

    for (it_init(it, g, v); it_valid(it); it_next(it))
    {
	size_t end = it_end(it);
	if (!visited[end])
	{
	    add_edge(mst, v, end, 1);
	    _dfs(g, end, visited, mst);
	}
    }
    free(it);
}

graph *dfsspan(graph *g)
{
    if (directed(g))
    {
	fprintf(stderr, "graph is directed\n");
	return NULL;
    }
    
    unsigned char *visited = calloc(sizeof(unsigned char), vertices(g));
    graph *ret = make_graph(vertices(g), 0);
    
    for (size_t i = 0; i < vertices(g); i++)
    {
	if (!visited[i])
	{
	    _dfs(g, i, visited, ret);
	}
    }

    free(visited);
    return ret;
}

graph *bfsspan(graph *g)
{
    if (directed(g))
    {
	fprintf(stderr, "graph is directed\n");
	return NULL;
    }

    unsigned char *visited = calloc(sizeof(unsigned char), vertices(g));
    graph *ret = make_graph(vertices(g), 0);
    queue *q = make_queue(g);
    iterator *it = make_iter();
    
    for (size_t i = 0; i < vertices(g); i++)
    {
	if (!visited[i])
	{
	    enqueue(q, i);
	    while (!qempty(q))
	    {
		size_t cur = dequeue(q);
		for (it_init(it, g, cur); it_valid(it); it_next(it))
		{
		    size_t end = it_end(it);
		    if (!visited[end])
		    {
			add_edge(ret, cur, end, 1);
			visited[end] = 1;
			enqueue(q, end);
		    }
		}
	    }
	}
    }

    free(it);
    dest_queue(q);
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

    char ch;
    printf("Choose the algorithm (d - DFS, b - BFS)\n");
    do
    {
	ch = getchar();
    } while (ch != 'd' && ch != 'b' && printf("Try again\n"));

    graph *span = (ch == 'd' ? dfsspan : bfsspan)(g);
    printf("Spanning tree:\n");
    writeg(span);
    
    dest_graph(g);
    dest_graph(span);
    
    return 0;
}
