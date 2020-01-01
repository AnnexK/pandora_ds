#pragma once

#include "interface.hpp"


namespace pandora_ds
{
    template <class T>
    class Tournamentsort : public ISorter<T>
    {
    public:
	virtual unsigned int sort(T* arr, int n) override
	{
	    T* tree = new T[2*n-1];
	    bool* filled = new bool[2*n-1];
	    for (int i = 0; i < 2*n-1; ++i)
		filled[i] = true;

	    for (int i = 2*n-2; i >= 0; i--)
	    {
		tree[i] = i < n-1 ? std::min(tree[2*i+1], tree[2*i+2]) : arr[i-n+1];
	    }

	    unsigned int cmp = n-1; // n-1 сравнений сверху

	    for (int i = 0; i < n; i++)
	    {
		arr[i] = tree[0];
		int j = 0;

		while (j < n-1)
		{
		    j = tree[j] == tree[2*j+1] ? 2*j+1 : 2*j+2;
		    cmp++;
		}

		filled[j] = false;

		while (j)
		{
		    j = (j - 1) / 2;
		    if (!filled[2*j+1] && !filled[2*j+2])
			filled[j] = false;
		    else
			tree[j] = compare(tree[2*j+1], tree[2*j+2], filled[2*j+1], filled[2*j+2]);
		    cmp++;
		}
	    }

	    delete[] tree;
	    delete[] filled;

	    return cmp;
	}

    private:
	const T& compare(const T& t1, const T& t2, bool f1, bool f2)
	{
	    if (!f1)
		return t2;
	    else if (!f2)
		return t1;
	    else return std::min(t1, t2);
	}
    };
}
