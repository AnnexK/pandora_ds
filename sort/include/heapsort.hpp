#pragma once

#include <cstddef>
#include "interface.hpp"


namespace pandora_ds
{
    template <class T>
    class Heapsort : public ISorter<T>
    {
    public:
	virtual unsigned int sort(T* arr, int n) override
	{
	    unsigned int cmp = build_heap(arr, n);

	    int size = n;
	    while (size)
	    {
		std::swap(arr[0], arr[--size]);
		cmp += heapify(arr, 0, size);
	    }
	    return cmp;
	}

    private:
	unsigned int build_heap(T* arr, int n)
	{
	    unsigned int cmp = 0;
	    for (int i = n / 2 - 1; i != -1; i--)
		cmp += heapify(arr, i, n);
	    return cmp;
	}

	unsigned int heapify(T* arr, int rt, int n)
	{
	    unsigned int cmp = 0;
	    int left = 2*rt + 1, right = 2*rt + 2;
	    int largest;

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
