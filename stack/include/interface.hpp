#pragma once


namespace pandora_ds
{
    template <class T>
    class IStack
    {
    public:
	IStack() = default;
	IStack(const IStack&) = delete;
	IStack(IStack&&) = delete;
	virtual ~IStack() = default;
	
	virtual IStack& operator=(const IStack&) = delete;
	virtual IStack& operator=(IStack&&) = delete;

	virtual void push(const T&) = 0;
	virtual T pop() = 0;
	virtual const T& peek() const = 0;
	virtual bool empty() const = 0;
    };
}
