//#include "array.h"
#include "linked.h"
#include <stdlib.h>
#include <stdio.h>


int main(void)
{
    stack *S = make_stack();

    push(S, 1);
    push(S, 2);

    int res;
    pop(S, &res);

    printf("%d", res);

    printf("%d", empty(S));

    pop(S, &res);
    printf("%d", res);

    printf("%d", empty(S));


    dest_stack(S);
    return 0;
}
