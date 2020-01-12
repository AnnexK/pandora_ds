#include "intsort.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void sort(unsigned int *arr, size_t size)
{
    unsigned int min = arr[0], max = 0;

    for (size_t i = 0; i < size; i++)
    {
	if (arr[i] < min)
	    min = arr[i];
	if (arr[i] > max)
	    max = arr[i];
    }

    size_t buffer_len = max-min+1;
    unsigned int *b = malloc(size*sizeof(unsigned int));
    unsigned int *c = calloc(buffer_len, sizeof(unsigned int));
    if (!b || !c)
    {
	fprintf(stderr, "could not allocate buffer\n");
	free(b);
	free(c);
	return;
    }

    for (size_t i = 0; i < size; i++)
	c[arr[i]-min]++;

    for (size_t i = 1; i < buffer_len; i++)
	c[i] += c[i-1];

    for (size_t i = 1; i <= size; i++)
    {
	b[c[arr[size-i]-min]-1] = arr[size-i];
	c[arr[size-i]-min]--;
    }

    memcpy(arr, b, sizeof(unsigned int)*size);
    free(b);
    free(c);
}
