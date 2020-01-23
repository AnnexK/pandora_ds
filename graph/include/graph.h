#ifndef DS_LAB7X8_GRAPH
#define DS_LAB7X8_GRAPH

#include <stddef.h>

#define SUCCESS 0
#define ALLOC_ERROR 1
#define EDGE_NOT_FOUND 2

struct _graph;
typedef struct _graph graph;

graph *make_graph(size_t, unsigned char);
void dest_graph(graph *);

size_t vertices(graph *);

int add_edge(graph *, size_t, size_t, int);
int get_edge(graph *, size_t, size_t);
int remove_edge(graph *, size_t, size_t);

#endif
