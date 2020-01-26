#ifndef DS_LAB7X8_GRAPH
#define DS_LAB7X8_GRAPH

#include <stddef.h>

#define SUCCESS 0
#define ALLOC_ERROR 1
#define EDGE_NOT_FOUND 2

struct _graph;
typedef struct _graph graph;

// объявление переменной, содержащей значение,
// сигн. об отсутствии ребра в структуре данных
// определена в файлах исходного кода структур данных
// как 0 для невзвешенных графов
// и как INT_MAX для взвешенных графов
extern const int no_edge;

graph *make_graph(size_t, unsigned char);
graph *copy_graph(graph *);
void dest_graph(graph *);

size_t vertices(graph *);
unsigned char directed(graph *);

int add_edge(graph *, size_t, size_t, int);
int get_edge(graph *, size_t, size_t);
int remove_edge(graph *, size_t, size_t);

#endif
