//#include "array.h"
#include "linked.h"

#include <stdio.h>


int main(void)
{
    queue *q = make_queue();
    int tmp;

    enqueue(q, 1);
    enqueue(q, 2);
    enqueue(q, 4);

    while (!empty(q))
    {
	dequeue(q, &tmp);
	printf("%d\n", tmp);
    }

    dest_queue(q);
    return 0;
}
