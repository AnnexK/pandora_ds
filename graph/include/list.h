#ifndef DS_LAB3_LIST
#define DS_LAB3_LIST

#define MEM_ERROR 1
#define SUCCESS 0
#define NODE_NOT_IN_LIST 2

#define CHECK_BELONG

#include "edge.h"

struct _node;
struct _list;

typedef struct _node node;
typedef struct _list list;

typedef int (*cmpf)(const void *, const void *);

list *make_list(size_t, cmpf);
void dest_list(list *);

node *first(list *);
void *data(node *);
node *next(node *);

int insert(list *, node *, void *);
node *search(list *, void *);
int delete(list *, node *);
int empty(list *);

void map(list *, void (*)(node *));

#endif
