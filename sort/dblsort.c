#include "doublesort.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

double *generate_array(size_t size, double min, double max)
{
    double *arr = malloc(sizeof(double) * size);

    for (size_t i = 0; i < size; i++)
    {
	double diff = (double)rand() * (max - min)/RAND_MAX;
	arr[i] = min + diff;
    }

    return arr;
}

int main(void)
{
    srand(time(NULL));

    size_t size = 200;
    double min = -100.0, max = 100.0;
    double *a = generate_array(size, min, max);

    printf("Original array:\n");
    for (size_t i = 0; i < size; i++)
    {
	printf("%g\n", a[i]);
    }
    putchar('\n');

    sort(a, size);

    printf("Sorted array:\n");
    for (size_t i = 0; i < size; i++)
    {
	printf("%g\n", a[i]);
    }
    putchar('\n');

    free(a);
    return 0;
}
