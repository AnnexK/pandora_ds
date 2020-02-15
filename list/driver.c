#include "list.h"

#include <stdio.h>

void print(node *n)
{
    unsigned int *d = data(n);
    printf("%u ", *d);
    if ( next(n) == NULL )
	putchar('\n');
}

int main(void)
{
    list *l = make_list(sizeof(unsigned int));

    for (unsigned int i = 0; i < 10; i++)
    {
	insert(l, NULL, &i);
    }

    unsigned int a = 7;
    delete(l, search(l, &a, NULL));
    a = 8;
    delete(l, search(l, &a, NULL));
    a = 6;
    delete(l, search(l, &a, NULL));
    delete(l, search(l, &a, NULL));
    a = 5;
    unsigned int b = 12;
    insert(l, search(l, &a, NULL), &b);
    map(l, print);

    dest_list(l);

    return 0;
}
