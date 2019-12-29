#pragma once

#include "interface.hpp"
#include <exception>


namespace pandora_ds
{   
    template <class T>
    class Shellsort : public ISorter<T>
    {
    public:
	class IShellGenerator
	{
	public:
	    IShellGenerator() = default;
	    virtual ~IShellGenerator() = default;

	    virtual int next() = 0;
	};

	using gen_ptr = std::unique_ptr<IShellGenerator>;
    
	class IShellGeneratorFactory
	{
	public:
	    IShellGeneratorFactory() = default;
	    virtual ~IShellGeneratorFactory() = default;

	    virtual gen_ptr make_gen(int n) = 0;
	};

	using fac_ptr = std::shared_ptr<IShellGeneratorFactory>;
    
	class DefaultShellGenerator : public IShellGenerator
	{
	public:
	    DefaultShellGenerator(int n)
		: current(n)
	    {}

	    virtual int next() override
	    {
		current /= 2;
		return current;
	    }
	private:
	    int current;
	};

	class DefaultShellGeneratorFactory : public IShellGeneratorFactory
	{
	public:
	    virtual gen_ptr make_gen(int n) override
	    {
		return std::make_unique<DefaultShellGenerator>(n);
	    }	
	};
	Shellsort(fac_ptr fac = std::make_shared<DefaultShellGeneratorFactory>())
	    : gen_fac (fac)
	{}
	
	virtual unsigned int sort(T* arr, int n) override
	{
	    unsigned int cmp = 0;
	    std::unique_ptr<IShellGenerator> gen = gen_fac->make_gen(n);
	    int d;
	    while ((d = gen->next()) > 0)
	    {
		for (int j = d; j < n; j++)
		{
		    T cur = arr[j];
		    int cur_index = j - d;
		    while (cur_index >= 0)
		    {
			cmp++;
			if (arr[cur_index] > cur)
			{
			    arr[cur_index+d] = arr[cur_index];
			    cur_index -= d;
			}
			else break;
		    }
		    arr[cur_index+d] = cur;
		}
	    }
	    return cmp;
	}
    private:
	fac_ptr gen_fac;
    };
}
