#include "cmpsort.h"

unsigned int sort(void *base,
		  size_t n,
		  size_t size,
		  int (*cmp)(const void *, const void *))
{
    unsigned int ret = 0;
    char *arr = base;
    for (size_t sorted_size = 0; sorted_size < n-1; sorted_size++)
    {
	char *min = arr + sorted_size * size;
	for (size_t i = sorted_size+1; i < n; i++)
	{
	    ret++;
	    if (cmp(min, arr + i * size) == 1)
	    {
		min = arr + i * size;
	    }
	}
	swap(arr + sorted_size * size, min, size);	
    }
    return ret;
}
