#include "cmpsort.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

void writeFile(FILE *fp, double *arr, size_t amt)
{
    for (int i = 0; i < amt; ++i)
    {
	fprintf(fp, "%g ", arr[i]);	
    }
    fprintf(fp, "\n");
}

int double_compare(const void *a, const void *b)
{
    double diff = *(double *)a - *(double *)b;
    if (diff < 0.0)
	return -1;
    else if (diff > 0.0)
	return 1;
    else return 0;
}

int main(void)
{
    char *choices[3] = {"file100.txt", "file1k.txt", "file10k.txt"};
    size_t sizes[3] = {100, 1000, 10000};
    FILE *fp;
    int choice;
    
    printf("Choose one of the following:\n1. 100 numbers\n2. 1K numbers\n3. 10K numbers\n");
    do
    {
	scanf("%d", &choice);
    } while ((choice < 1 || choice > 3) && printf("Try again\n"));

    fp = fopen(choices[choice - 1], "r");
    size_t size = sizes[choice-1];
    
    double *arr = malloc(size * sizeof(double));
    for (int i = 0; i < size; ++i)
    {
	fscanf(fp, "%lg", arr + i);
    }

    fclose(fp);
    
    unsigned int cmps = sort(arr, size, sizeof(double), double_compare);
    
    fp = fopen("result.txt", "w");
    writeFile(fp, arr, size);
    fclose(fp);

    printf("Amount of comparisons: %d\n", cmps);

    free(arr);
    return 0;
}
