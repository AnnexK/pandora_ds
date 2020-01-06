#include "cmpsort.h"
#include "swap.h"

unsigned int sort(void *base,
		  size_t n,
		  size_t size,
		  int (*cmp)(const void *, const void *))
{
    unsigned int res = 0;

    char *arr = base;
    for (size_t len = n; len; len--)
    {
	for (size_t cur = 0; cur < len-1; cur++)
	{
	    char *a = arr + (cur+1) * size;
	    char *b = arr + cur * size;
	    if (cmp(a, b) == -1)
	    {
		swap(a, b, size);
	    }
	    res++;
	}
    }
    return res;
}
