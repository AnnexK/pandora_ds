#include "intsort.h"

#include "queue.h"

unsigned int key(unsigned int c, unsigned int shift)
{
    return (c / shift) % 10;
}

void sort(unsigned int *arr, size_t size)
{
    queue **qs = malloc(10*sizeof(queue *));
    for (size_t i = 0; i < 10; i++)
    {
	qs[i] = make_queue();
    }

    unsigned int max = 0;
    for (size_t i = 0; i < size; i++)
    {
	if (arr[i] > max)
	    max = arr[i];
    }

    for (unsigned int k = 1; k <= max; k *= 10)
    {
	for (size_t i = 0; i < size; i++)
	{
	    enqueue(qs[key(arr[i], k)], arr[i]);
	}
	size_t arr_i = 0;
	for (size_t i = 0; i < 10; i++)
	{
	    while ( !empty(qs[i]) )
	    {
		dequeue(qs[i], arr+arr_i);
		arr_i++;
	    }
	}
    }
    
    for (size_t i = 0; i < 10; i++)
    {
	dest_queue(qs[i]);
    }
    free(qs);    
}
