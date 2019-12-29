#pragma once

#include <cstddef>
#include "interface.hpp"

namespace pandora_ds
{
    template <class T>
    class Insertionsort : public ISorter<T>
    {
	virtual unsigned int sort(T* arr, int n) override
	{
	    unsigned int cmp = 0;
	    int sorted_size = 1;
	    while (sorted_size < n)
	    {
		T cur = arr[sorted_size];
		int cur_index = sorted_size-1;
		while (cur_index >= 0)
		{
		    cmp++;
		    if (arr[cur_index] > cur)
		    {
			arr[cur_index+1] = arr[cur_index];
			cur_index--;
		    }
		    else break;
		}
		arr[cur_index+1] = cur;
		sorted_size++;
	    }
	    return cmp;
	}
    };
}
