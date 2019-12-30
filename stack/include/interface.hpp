#pragma once


namespace pandora_ds
{
    /* АТД Стек */
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

	/* Интерфейс операций АТД Стек */
	// Поместить эл-т в стек
	virtual void push(const T&) = 0;
	// Извлечь эл-т из стека
	virtual T pop() = 0;
	// Просмотреть вершину стека
	virtual const T& peek() const = 0;
	// Проверить стек на пустоту
	virtual bool empty() const = 0;
    };
}
