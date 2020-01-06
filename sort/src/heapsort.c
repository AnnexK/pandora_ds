#include "cmpsort.h"


unsigned int heapify(void *base,
		     char *root,
		     size_t n,
		     size_t size,
		     cmpfoo f)
{
    unsigned int cmp = 0;
    char *arr = base;
    char *left = root + (root - arr) + size;
    char *right = left + size;
    char *largest;

    if (left < arr + n * size)
    {
	largest = f(left, root) == 1 ? left : root;
	cmp++;
    }
    else
    {
	largest = root;
    }

    if (right < arr + n * size)
    {
	if ( f(right, largest) == 1 )
	    largest = right;
	cmp++;
    }

    if (largest != root)
    {
	swap(root, largest, size);
	cmp += heapify(base, largest, n, size, f);
    }
    return cmp;
}

unsigned int build_heap(void *base,
			size_t n,
			size_t size,
		        cmpfoo f)
{
    unsigned int cmp = 0;
    for (char *root = (char *)base + (n/2-1) * size; root >= (char *)base; root -= size)
	cmp += heapify(base, root, n, size, f);
    return cmp;
}

unsigned int sort(void *base,
		  size_t n,
		  size_t size,
		  cmpfoo f)
{
    unsigned int cmp = build_heap(base, n, size, f);

    size_t heap_size = n;
    while (heap_size > 0)
    {
	swap(base, (char *)base + (heap_size - 1)*size, size);
	heap_size--;
	cmp += heapify(base, base, heap_size, size, f);
    }
    return cmp;
}
