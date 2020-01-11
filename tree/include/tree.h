#ifndef DS_LAB4_TREE
#define DS_LAB4_TREE


typedef struct _tree
{
    unsigned int data;
    struct _tree *left, *right; 
} tree;

tree *make_tree(unsigned int);
void dest_tree(tree *);

tree *insert(tree *, unsigned int);
tree *search(tree *, unsigned int);
tree *delete(tree *, unsigned int);

void prefix(tree *, void (*)(tree *));
void postfix(tree *, void(*)(tree *));
void infix(tree *, void(*)(tree *));

#endif
