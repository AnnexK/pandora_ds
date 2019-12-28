#pragma once

#include <cstddef>
#include <iostream>
#include "interface.hpp"

namespace pandora_ds
{
    template <class T>
    class Mergesort : public ISorter<T>
    {
    public:
	virtual unsigned int sort(T* arr, size_t n) override
	{
	    unsigned int res = 0;

	    /* буфер для слияний */
	    T* buf = new T[n];
	    /* сначала сливаем подмассивы длиной 1 */
	    size_t size = 1;

	    while (size < n)
	    {
		size_t left = 0;
		while (left < n)
		{
		    size_t mid = std::min(left + size - 1, n-1);
		    /* если последний подмассив короче */
		    size_t right = std::min(mid + size, n-1);
		    res += merge(arr, buf, left, mid, right);
		    /* перемещаемся к следующей паре подмассивов */
		    left += 2*size;
		}
		/* а потом увеличиваем длины подмассивов вдвое */
		size *= 2;
	    }

	    delete[] buf;
	    return res;
	}
    private:
	/* совершает слияние 
	   (отсортированных) подмассивов A[l..m] и A[m+1..r],
	   используя буфер B 
	   возвращает кол-во сравнений */
        unsigned int merge(T* A, T* B, size_t l, size_t m, size_t r)
	{
	    size_t buf_cur = 0, left_cur = l, right_cur = m+1;
	    unsigned int cmp = 0;

	    while (left_cur <= m && right_cur <= r)
	    {
		B[buf_cur++] = A[left_cur] < A[right_cur] ? A[left_cur++] : A[right_cur++];      
		cmp++;
	    }

	    /* выполнится только одна */
	    while (left_cur <= m)
	    {
		B[buf_cur++] = A[left_cur++];
	    }
	    while (right_cur <= r)
	    {
		B[buf_cur++] = A[right_cur++];
	    }

	    for (size_t i = l; i <= r; i++)
	    {
		A[i] = B[i-l];
	    }

	    return cmp;
	}
    };
}
