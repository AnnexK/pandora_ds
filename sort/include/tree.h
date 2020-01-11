#ifndef DS_LAB6_TREESORT
#define DS_LAB6_TREESORT

typedef struct _tree
{
    void *data;
    struct _tree *left, *right; 
} tree;

tree *make_tree(void *);
void dest_tree(tree *);

tree *insert(tree *, void *, int(*)(const void *, const void *));

void infix(tree *, void(*)(tree *));

#endif
