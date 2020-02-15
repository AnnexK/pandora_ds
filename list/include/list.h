#ifndef DS_LAB3_LIST
#define DS_LAB3_LIST

#include <stddef.h>

#define MEM_ERROR 1
#define SUCCESS 0
#define NODE_NOT_IN_LIST 2

#define CHECK_BELONG

struct _node;
struct _list;

typedef struct _node node;
typedef struct _list list;

list *make_list(size_t);
void dest_list(list *);

node *first(list *);
node *last(list *);
void *data(node *);
node *next(node *);

int insert(list *, node *, const void *);
node *search(list *, const void *, int (*)(const void *, const void *));
int delete(list *, node *);
int empty(list *);

void map(list *, void (*)(node *));

#endif
