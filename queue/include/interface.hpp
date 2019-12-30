#pragma once


namespace pandora_ds
{
    /* АТД Очередь */
    template <class T>
    class IQueue
    {
    public:
	IQueue() = default;
	IQueue(const IQueue&) = delete;
	IQueue(IQueue&&) = delete;
	virtual ~IQueue() = default;

	IQueue& operator=(const IQueue&) = delete;
	IQueue& operator=(IQueue&&) = delete;

	/* Интерфейс операций АТД Очередь */

	// Помещение эл-та в конец очереди
	virtual void enqueue(const T&) = 0;
	// Извлечение эл-та из начала очереди
	virtual T dequeue() = 0;
	// Просмотр эл-та в начале очереди
	virtual const T& peek() const = 0;
	// Проверка очереди на пустоту
	virtual bool empty() const = 0;
    };
}
