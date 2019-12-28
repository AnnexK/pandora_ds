#pragma once

#include <cstddef>
#include "interface.hpp"


namespace pandora_ds
{
    template <class T>
    class Heapsort : public ISorter<T>
    {
    public:
	virtual unsigned int sort(T* arr, size_t n) override
	{
	    unsigned int cmp = build_heap(arr, n);

	    size_t size = n;
	    while (size)
	    {
		std::swap(arr[0], arr[--size]);
		cmp += heapify(arr, 0, size);
	    }
	    return cmp;
	}

    private:
	unsigned int build_heap(T* arr, size_t n)
	{
	    unsigned int cmp = 0;
	    // грязно, конечно, но кто будет в здравом уме
	    // сортировать в ОЗУ массив длиной 18 KKKKKK записей?
	    for (size_t i = n / 2 - 1; i != (size_t)-1; i--)
		cmp += heapify(arr, i, n);
	    return cmp;
	}

	unsigned int heapify(T* arr, size_t rt, size_t n)
	{
	    unsigned int cmp = 0;
	    size_t left = 2*rt + 1, right = 2*rt + 2;
	    size_t largest;

	    if (left < n)
	    {
		largest = arr[left] > arr[rt] ? left : rt;
		cmp++;
	    }
	    else
	    {
		largest = rt;
	    }
	
	    if (right < n)
	    {
		if (arr[right] > arr[largest])
		    largest = right;
		cmp++;
	    }

	    if (largest != rt)
	    {
		std::swap(arr[rt], arr[largest]);
		cmp += heapify(arr, largest, n);
	    }
	    return cmp;
	}
    };
}
