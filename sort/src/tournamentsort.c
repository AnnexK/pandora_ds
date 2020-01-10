#include "cmpsort.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


char *_tour_compare(char *left, char *right,
		    char left_x, char right_x,
		    cmpfoo f)
{
    if (!left_x)
	return right;
    else if (!right_x)
	return left;
    else if ( f(left, right) < 1 )
	return left;
    else return right;
}

unsigned int sort(void *base,
		  size_t n,
		  size_t size,
		  cmpfoo f)
{
    unsigned int cmp = 0;
    char *arr = base;
    char *tour = malloc( (2*n-1)*size );
    char *exists = malloc( (2*n-1)*size );

    if (!tour || !exists)
    {
	free(tour);
	free(exists);
	fprintf(stderr, "could not allocate buffers");
	return (unsigned int)-1;
    }
    // сначала все элементы турнира существуют
    memset(exists, 1, (2*n-1)*size);

    // заполняем нижний ярус дерева элементами
    memcpy(tour+(n-1)*size, arr, n*size);
    // заполняем остальное дерево
    for (char *cur = tour+(n-2)*size; cur >= tour; cur -= size)
    {
	cmp++;
	char *left = cur+(cur-tour+size), *right = left+size;
	memcpy(cur, f(left, right) == -1 ? left : right, size);
    }

    for (size_t i = 0; i < n; i++)
    {
	memcpy(arr+i*size, tour, size);
	size_t j = 0;

	while (j < n-1)
	{
	    if ( f(tour+j*size, tour+(2*j+1)*size) == 0 )
		j = 2*j+1;
	    else
		j = 2*j+2;
	}

	exists[j] = 0;

	while (j)
	{
	    j = (j-1) / 2;
	    if (!exists[2*j+1] && !exists[2*j+2])
		exists[j] = 0;
	    else
	    {
		char *left = tour+(2*j+1)*size;
		char *right = left+size;
		char *write = _tour_compare(left, right, exists[2*j+1], exists[2*j+2], f);
		memcpy(tour+j*size, write, size);
	    }
	    cmp++;
	}
    }

    free(tour);
    free(exists);
    return cmp;
}
