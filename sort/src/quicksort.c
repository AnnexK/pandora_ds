#include "cmpsort.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

size_t make_pivot(size_t left, size_t right)
{
    return (rand() % (right-left+1)) + left;
}

unsigned int partition(void *base,
		       size_t n,
		       size_t size,
		       cmpfoo f,
		       size_t left,
		       size_t right,
		       size_t *q)
{
    unsigned int cmp = 0;
    char *arr = base;
    
    char *q_val = arr + make_pivot(left,right)*size;

    char piv_val[size];
    memcpy(piv_val, q_val, size);

    char *i = arr + (left-1)*size;
    char *j = arr + (right+1)*size;

    while (1)
    {
	do
	{
	    cmp++;
	    j -= size;
	} while( f(j, piv_val) == 1 );
	do
	{
	    cmp++;
	    i += size;
	} while ( f(i, piv_val) == -1 );

	if (i < j)
	    swap(i, j, size);
	else
	{
	    *q = (j-arr) / size;
	    return cmp;
	}
    }
}

unsigned int _qsort(void *base,
		    size_t n,
		    size_t size,
		    cmpfoo f,
		    size_t left,
		    size_t right)
{
    if (left >= right) return 0;
    size_t q;
    unsigned int cmp = partition(base, n, size, f, left, right, &q);
    return _qsort(base, n, size, f, left, q) + _qsort(base, n, size, f, q+1, right) + cmp;
}

unsigned int sort(void *base,
		  size_t n,
		  size_t size,
		  cmpfoo f)
{
    srand(time(NULL));
    return _qsort(base, n, size, f, 0, n-1);
}

