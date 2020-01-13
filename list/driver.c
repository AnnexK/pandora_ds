#include "list.h"

#include <stdio.h>

void print(node *n)
{
    T *d = data(n);
    printf("%u ", *d);
    if ( next(n) == NULL )
	putchar('\n');
}

int main(void)
{
    list *l = make_list();

    for (unsigned int i = 0; i < 10; i++)
    {
	insert(l, NULL, i);
    }

    delete(l, search(l, 7));
    delete(l, search(l, 8));
    delete(l, search(l, 6));
    delete(l, search(l, 6));
    insert(l, search(l, 5), 12);
    map(l, print);

    dest_list(l);

    return 0;
}
