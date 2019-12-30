#pragma once

#include "interface.hpp"
#include <stdexcept>

namespace pandora_ds
{
    /* Стек на массиве */
    template <class T>
    class ArrayStack : public IStack<T>
    {
    public:
	ArrayStack(int size = 128)
	    : max_size(size), current(-1)
	{
	    data = new T[size];
	}

	ArrayStack(const ArrayStack& rhs) = delete;
	ArrayStack(ArrayStack&& rhs) = delete;
	
	virtual ~ArrayStack() override
	{
	    delete[] data;
	}

        ArrayStack& operator=(const ArrayStack& rhs) = delete;
	ArrayStack& operator=(ArrayStack&& rhs) = delete;

	virtual void push(const T& val) override
	{
	    if (current == max_size - 1)
		throw std::runtime_error("Stack overflow");
	    data[++current] = val;
	}

	virtual T pop() override
	{
	    T ret = peek();
	    current--;
	    return ret;
	}

	virtual const T& peek() const override
	{
	    if (empty())
		throw std::runtime_error("Stack underflow");
	    return data[current];
	}

	virtual bool empty() const override
	{
	    return current == -1;
	}
    private:
	// массив
	T* data;
	// макс. размер массива
	int max_size;
	// индекс текущего эл-та
	int current;
    };
}
