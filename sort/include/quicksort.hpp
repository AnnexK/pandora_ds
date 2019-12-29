#pragma once

#include "interface.hpp"
#include <iostream>
#include <memory>
#include <cstdlib>
#include <ctime>


namespace pandora_ds
{
    template <class T>
    class Quicksort : public ISorter<T>
    {
    public:
	/* Интерфейс для классов, выбирающих индекс разделяющего элемента
	   Предназначен для реализации различных алгоритмов выбора РЭ */
	class IPivotSelector
	{
	public:
	    IPivotSelector() = default;
	    virtual ~IPivotSelector() = default;

	    virtual int index(int, int) = 0;
	};

	/* Класс, выбирающий случайный элемент массива
	   в качестве разделяющего */
	class RandomPivot : public IPivotSelector
	{
	public:
	    RandomPivot()
	    {
		std::srand(std::time(nullptr));
	    }

	    virtual int index(int l, int r) override
	    {
		return (std::rand() % (r - l + 1)) + l;
	    }
	};

	/* Указатель на объект выбиральщика РЭ */
	using selector_ptr = std::shared_ptr<IPivotSelector>;

	/* По умолчанию выбираем RandomPivot */
	Quicksort(selector_ptr ps = std::make_shared<RandomPivot>())
	    : selector(ps)
	{}

	virtual unsigned int sort(T* arr, int n) override
	{
	    return qsort(arr, 0, n-1);
	}
    private:
	// Выбиральщик
	selector_ptr selector;
	
	unsigned int qsort(T* arr, int l, int r)
	{
	    if (l >= r) return 0;
	    int q;
	    unsigned int cmp = partition(arr, l, r, q);
	    return qsort(arr, l, q) + qsort(arr, q+1, r) + cmp;
	}

	unsigned int partition(T* arr, int l, int r, int& pivot)
	{
	    unsigned int cmp = 0;
	    pivot = selector->index(l, r);
	    std::swap(arr[l], arr[pivot]);
	    T piv_value = arr[pivot];

	    int i = l - 1, j = r + 1;

	    while (true)
	    {
		do
		{
		    cmp++;
		    j--;
		} while (arr[j] > piv_value);

		do
		{
		    cmp++;
		    i++;
		} while (arr[i] < piv_value);

		if (i < j)
		    std::swap(arr[i], arr[j]);
		else
		{
		    pivot = j;
		    return cmp;
		}
	    }
	}
    };
}
