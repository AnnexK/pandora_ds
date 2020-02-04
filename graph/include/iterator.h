#ifndef DS_LAB7X8_ADJACENCY_ITERATOR
#define DS_LAB7X8_ADJACENCY_ITERATOR

/* Как работать с итератором:
   1. С помощью make_iter создайте итератор в начале алгоритма
   2. Перед использованием итератора для прохода по множеству смежных вершин инициализируйте итератор функцией it_init, передав в нее итератор, граф и вершину
   3. Перемещайтесь по списку смежности, используя it_next
   4. Для определения конца итератора используйте функцию it_valid
   5. Функции it_start и it_end возвращают соответственно начальную и конечную вершину или количество вершин в графе, если итератор исчерпан
   6. Функция it_data возвращает данные ребра или гарантировано неверное значение при исчерпании итератора (INFINITY)
   7. После завершения работы с итератором освободите возвращенный make_iter указатель, передав его функции free */

#include <stddef.h>
#include "graph.h"

struct _iterator;
typedef struct _iterator iterator;

iterator *make_iter(void);
void it_init(iterator *, graph *, size_t);

size_t it_start(iterator *);
size_t it_end(iterator *);
double it_data(iterator *);

void it_next(iterator *);
int it_valid(iterator *);

#endif
