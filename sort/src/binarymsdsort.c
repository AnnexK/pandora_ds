/* NB: пока неизвестно, что именно нужно, кроме алг. сортировки */
#include <stddef.h>

unsigned char key(unsigned int k, size_t s)
{
    return (k >> (s-1)) & 1;
}

void _brs(unsigned int *left, unsigned int *right, size_t shift)
{
    if (left < right && shift)
    {
	unsigned int *l_arr = left-1;
	unsigned int *r_arr = right+1;
	while (l_arr < r_arr)
	{
	    do
	    {
		r_arr--;
	    } while (key(*r_arr, shift) == 1 && r_arr > l_arr);

	    do
	    {
		l_arr++;
	    } while (key(*l_arr, shift) == 0 && l_arr < r_arr);

	    if (l_arr < r_arr)
	    {
		unsigned int tmp = *l_arr;
		*l_arr = *r_arr;
		*r_arr = tmp;
	    }
	}
	_brs(left, r_arr, shift-1);
	_brs(r_arr+1, right, shift-1);
    }
}

void sort(unsigned int *arr, size_t size)
{
    _brs(arr, arr+size-1, 8*sizeof(unsigned int));
}

