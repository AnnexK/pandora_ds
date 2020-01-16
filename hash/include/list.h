#ifndef DS_LAB3_LIST
#define DS_LAB3_LIST

#define MEM_ERROR 1
#define SUCCESS 0
#define NODE_NOT_IN_LIST 2

#define CHECK_BELONG

#include "pair.h"

typedef pair T;

struct _node;
struct _list;

typedef struct _node node;
typedef struct _list list;

list *make_list(void);
void dest_list(list *);

node *first(list *);
T *data(node *);
node *next(node *);

int linsert(list *, node *, T);
int ldelete(list *, node *);
int empty(list *);

void map(list *, void (*)(node *));

#endif
