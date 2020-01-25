#ifndef DS_LAB7X8_GRAPH_IO
#define DS_LAB7X8_GRAPH_IO

#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

#define FILE_ERROR 1
#define GRAPH_NULL 2
#define WRITE_SUCCESS 0

// выдавать ошибку в случае нахождения дубликата ребра в файле
// закомментируйте define, если такое поведение нежелательно
// в этом случае дубликаты ребра будут игнорироваться
#define STRICT_NONREPEAT

// ввод-вывод на консоль
#define writeg(G) fwriteg(stdout, (G))
#define readg(G) freadg(stdin, (G))

int fwriteg(FILE *, graph *);
graph *freadg(FILE *);

#endif
