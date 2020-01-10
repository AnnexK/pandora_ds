#include "cmpsort.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MIN(A, B) ((A) < (B) ? (A) : (B))

unsigned int merge(void *base,
		   char *buffer,
		   size_t left,
		   size_t mid,
		   size_t right,
		   size_t size,
		   cmpfoo f)
{
    char *arr = base;
    char *buf_cur = buffer;
    char *left_cur = arr + left*size;
    char *right_cur = arr + (mid+1)*size;
    char *const left_border = arr + mid*size;
    char *const right_border = arr + right*size;
    unsigned int cmp = 0;
    
    while (left_cur <= left_border && right_cur <= right_border)
    {
	if ( f(left_cur, right_cur) == -1 )
	{
	    memcpy(buf_cur, left_cur, size);
	    left_cur += size;
	}
	else
	{
	    memcpy(buf_cur, right_cur, size);
	    right_cur += size;
	}
	cmp++;
	buf_cur += size;
    }

    /* копировать остатки в буфер,
       копирован будет только один подмассив,
       т. к. для одного из вызовов memcpy
       size = 0 */
    memcpy(buf_cur, left_cur, left_border+size-left_cur);
    memcpy(buf_cur, right_cur, right_border+size-right_cur);

    /* копировать содержимое буфера обратно в массив */
    memcpy(arr+left*size, buffer, (right-left+1)*size);
    return cmp;
}
unsigned int sort(void *base,
		  size_t n,
		  size_t size,
		  cmpfoo f)
{
    unsigned int cmp = 0;

    char *buf = malloc(n * size);
    if (!buf)
    {
	fprintf(stderr, "could not allocate buffer");
	return (unsigned int)-1;
    }
    size_t ssize = 1;

    while (ssize < n)
    {
	size_t left = 0;
	while (left < n)
	{
	    size_t mid = MIN(left+ssize-1, n-1);
	    size_t right = MIN(mid+ssize, n-1);
	    cmp += merge(base, buf, left, mid, right, size, f);
	    left += 2*ssize;
	}
	ssize *= 2;
    }

    free(buf);
    return cmp;
}

