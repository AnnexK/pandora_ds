#include "tree.h"
#include <stdio.h>

void print(tree *t)
{
    printf("%u ", t->data);
}

int main(void)
{
    tree *t = NULL;

    for (unsigned int i = 0; i < 10; i++)
	t = insert(t, i);
    infix(t, print);
    putchar('\n');
    
    t = insert(t, 120);
    infix(t, print);
    putchar('\n');

    t = delete(t, 9);
    infix(t, print);
    putchar('\n');

    tree *eight = search(t, 8);
    if (eight->left)
	print(eight->left);
    if (eight->right)
	print(eight->right);

    tree *nine = search(t, 9);
    if (!nine)
	printf("\nNo 9 in tree xd\n");
    dest_tree(t);
    return 0;
}
