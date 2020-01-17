#include "hash.h"
#include "chain.h"
#include "functions.h"

#include <stdio.h>

int main(void)
{
    dictionary *dict = make_dict(31, division);

    insert(dict, 22, 147);
    insert(dict, 57, 42);

    print(dict);
    printf("%u\n", search(dict, 22)->value);
    printf("%u\n", search(dict, 57)->value);
    insert(dict, 22, 148);

    printf("%u\n", search(dict, 22)->value);

    delete(dict, 22);

    if ( !search(dict, 22) )
	printf("Deleted\n");

    dest_dict(dict);

    return 0;
}
