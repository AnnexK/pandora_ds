#include "hash.h"
#include "functions.h"

#include "dhash.h"

#include <stdio.h>

int main(void)
{
    dictionary *dict = make_dict(31, division, division);

    insert(dict, 22, 147);
    insert(dict, 57, 42);

    printf("%u\n", get_last_tries());

    insert(dict, 56, 42);
    printf("%u\n", get_last_tries());

    print(dict);

    dest_dict(dict);

    return 0;
}
