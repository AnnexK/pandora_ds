#include "cmpsort.h"

#include <string.h>

unsigned int sort(void *base,
		  size_t n,
		  size_t size,
		  int (*cmp)(const void *, const void *))
{
    unsigned int ret = 0;
    char *arr = base;
    char buf[size];
    
    for (size_t sorted_size = 1; sorted_size < n; sorted_size++)
    {
	memcpy(buf, arr+sorted_size*size, size);
	char *swp = arr+(sorted_size-1)*size;
	while (swp >= arr)
	{
	    ret++;
	    if (cmp(swp, buf) == 1)
	    {
		memcpy(swp+size, swp, size);
		swp -= size;
	    }
	    else break;
	}
	memcpy(swp+size, buf, size);	
    }

    return ret;
}
