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
	virtual unsigned int sort(T* arr, int n) override
	{
	    unsigned int res = 0;

	    /* буфер для слияний */
	    T* buf = new T[n];
	    /* сначала сливаем подмассивы длиной 1 */
	    int size = 1;

	    while (size < n)
	    {
		int left = 0;
		while (left < n)
		{
		    int mid = std::min(left + size - 1, n-1);
		    /* если последний подмассив короче */
		    int right = std::min(mid + size, n-1);
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
        unsigned int merge(T* A, T* B, int l, int m, int r)
	{
	    int buf_cur = 0, left_cur = l, right_cur = m+1;
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

	    for (int i = l; i <= r; i++)
	    {
		A[i] = B[i-l];
	    }

	    return cmp;
	}
    };
}
