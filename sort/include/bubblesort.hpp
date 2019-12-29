#pragma once

#include "interface.hpp"

namespace pandora_ds
{
    template <class T>
    class Bubblesort : public ISorter<T>
    {
    public:
	/* реализация интерфейса */
	virtual unsigned int sort(T* arr, int size) override
	{
	    unsigned int res = 0;
	    for (int len = size; len; len--)
	    {
		for (int cur = 0; cur < len-1; cur++)
		{
		    if (arr[cur+1] < arr[cur])
			std::swap(arr[cur], arr[cur+1]);
		    res++;
		}
	    }
	    return res;
	}
    };
}
