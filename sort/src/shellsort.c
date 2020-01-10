#include "cmpsort.h"

/* вычисление следующего шага 
   должно вернуть 0 при c == 1 */
size_t next_coeff(size_t c)
{
    return c / 2;
}

unsigned int sort(void *base,
		  size_t n,
		  size_t size,
		  cmpfoo f)
{
    unsigned int cmp = 0;
    char buffer[size];
    char *arr = base;
    
    for (size_t d = next_coeff(n); d; d = next_coeff(d))
    {
	for (size_t j = d; j < n; j++)
	{
	    memcpy(buffer, arr+size*j, size);
	    char *swp = arr+(j-d)*size;
	    while (swp >= arr)
	    {
		cmp++;
		if ( f(swp, buffer) == 1 )
		{
		    memcpy(swp+d*size, swp, size);
		    swp -= d*size;
		}
		else break;
	    }
	    memcpy(swp+d*size, buffer, size);
	}
    }
    return cmp;
}
