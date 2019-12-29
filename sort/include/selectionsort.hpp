#pragma once

#include "interface.hpp"

namespace pandora_ds
{
    template <class T>
    class Selectionsort : public ISorter<T>
    {
    public:
	virtual unsigned int sort(T* arr, int n) override
	{
	    unsigned int cmp = 0;
	    int sorted_size = 0;
	    while (sorted_size < n-1)
	    {
		T min = arr[sorted_size];
		int minidx = sorted_size;
		for (int i = sorted_size+1; i < n; i++)
		{
		    cmp++;
		    if (min > arr[i])
		    {
			min = arr[i];
			minidx = i;
		    }
		}
		std::swap(arr[sorted_size], arr[minidx]);
		sorted_size++;
	    }
	    return cmp;
	}
    };
}
