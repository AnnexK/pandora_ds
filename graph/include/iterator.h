#ifndef DS_LAB7X8_ADJACENCY_ITERATOR
#define DS_LAB7X8_ADJACENCY_ITERATOR

/* Как работать с итератором:
   1. С помощью make_iter создайте итератор, передав граф и вершину
   2. Перемещайтесь по списку смежности, используя it_next
   3. Для определения конца итератора проверяйте возвр. значение на равенство NULL
   4. Функции it_start и it_end возвращают соответственно начальную и конечную вершину или количество вершин в графе, если итератор исчерпан
   5. Функция it_data возвращает данные ребра или гарантировано неверное значение при исчерпании итератора (0 для невзв. графа, INT_MAX для взв. графа)
   6. Исчерпание итератора с помощью it_next освободит память (хотя, может, это и не самая хорошая идея); при досрочном выходе освободите память самостоятельно */

#include <stddef.h>
#include "graph.h"

#define STOP_ITERATION 1

struct _iterator;
typedef struct _iterator iterator;

iterator *make_iter(void);
void it_init(iterator *, graph *, size_t);

size_t it_start(iterator *);
size_t it_end(iterator *);
int it_data(iterator *);

void it_next(iterator *);
int it_valid(iterator *);

#endif
