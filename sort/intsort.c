#include "intsort.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

unsigned int *make_array(size_t size, unsigned int left, unsigned int right)
{
    unsigned int *ret = malloc(size*sizeof(int));
    if (!ret)
    {
	fprintf(stderr, "could not allocate array");
	return NULL;
    }
    
    for (size_t i = 0; i < size; i++)
    {
	ret[i] = left + rand()%(right-left+1);
    }
    return ret;
}

void print_array(unsigned int *arr, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
	printf("%u ", arr[i]);
    }
    printf("\n");
}

int main(void)
{
    srand(time(NULL));
    
    size_t size = 100;
    unsigned int l = 0, r = 1000;
    
    unsigned int *arr = make_array(size, l, r);
    
    printf("Pre-sort:\n");
    print_array(arr, size);
    
    sort(arr, size);
    
    printf("Post-sort:\n");
    print_array(arr, size);
    
    free(arr);
    return 0;
}
