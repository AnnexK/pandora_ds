#ifndef DS_LAB6_BUCKET_LIST
#define DS_LAB6_BUCKET_LIST

#define MEM_ERROR 1
#define SUCCESS 0
#define NODE_NOT_IN_LIST 2

#define CHECK_BELONG

typedef double T;

struct _node;
struct _list;

typedef struct _node node;
typedef struct _list list;

list *make_list(void);
void dest_list(list *);

node *first(list *);
T *data(node *);
node *next(node *);

int insert(list *, node *, T);
node *search(list *, T);
int delete(list *, node *);
int empty(list *);

void map(list *, void (*)(node *));
void lsort(list *);

#endif
