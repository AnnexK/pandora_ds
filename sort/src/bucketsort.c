#include "doublesort.h"
#include "list.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define BUCKET_AMOUNT 10

double *tmp;
size_t cur;

void arrcpy(node *n)
{
    tmp[cur] = *data(n);
    cur++;
}

void print(node *n)
{
    printf("%lf ", *data(n));
    if (!next(n))
	putchar('\n');
}

void sort(double *arr, size_t size)
{
    if (!size) return;
    tmp = arr;
    cur = 0;
    
    double min = arr[0], max = arr[0];

    for (size_t i = 0; i < size; i++)
    {
	if (min > arr[i])
	    min = arr[i];
	if (max < arr[i])
	    max = arr[i];
    }

    double diff = max-min;
    if ( abs(diff) < 1E-9 ) // все элементы одинаковые
	return;
    
    list **buckets = malloc(BUCKET_AMOUNT * sizeof(list *));
    for (size_t i = 0; i < BUCKET_AMOUNT; i++)
    {
	buckets[i] = make_list();
    }

    for (size_t i = 0; i < size; i++)
    {
	double m = (arr[i]-min) / diff;
	size_t key = (size_t)(m * BUCKET_AMOUNT);
	if (key == BUCKET_AMOUNT)
	    key--;
	insert(buckets[key], NULL, arr[i]);
    }

    for (size_t i = 0; i < BUCKET_AMOUNT; i++)
    {
	map(buckets[i], print);
	lsort(buckets[i]);
    }

    for (size_t i = 0; i < BUCKET_AMOUNT; i++)
    {
	map(buckets[i], arrcpy);
    }

    for (size_t i = 0; i < BUCKET_AMOUNT; i++)
	dest_list(buckets[i]);
    free(buckets);
}
