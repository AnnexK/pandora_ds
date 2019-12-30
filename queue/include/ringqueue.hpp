#pragma once

#include <stdexcept>
#include "interface.hpp"


namespace pandora_ds
{
    /* Очередь на массиве
       (СД Кольцевой Буфер) */
    template <class T>
    class RingBufferQueue : public IQueue<T>
    {
    private:
	// тип операции (вставка/удаление)
        enum op {ENQUEUE, DEQUEUE};
	// массив данных
	T* data;
	// индекс головы очереди (откуда брать)
	int head;
	// индекс хвоста очереди (куда пихать)
	int tail;
	// размер массива
	int size;
	// последняя операция
	// используется для определения пустоты/полноты очереди:
	// head == last и последняя операция E -- очередь полна
	// head == last и последняя операция D -- очередь пуста
	op last;
    public:
	RingBufferQueue(int size = 128)
	    // DEQUEUE для правильной работы empty, dequeue и enqueue
	    : head(0), tail(0), size(size), last(DEQUEUE)
	{
	    data = new T[size];
	}

	RingBufferQueue(const RingBufferQueue&) = delete;
	RingBufferQueue(RingBufferQueue&&) = delete;
	
	virtual ~RingBufferQueue() override
	{
	    delete[] data;
	}

	RingBufferQueue& operator=(const RingBufferQueue&) = delete;
	RingBufferQueue& operator=(RingBufferQueue&&) = delete;

	virtual void enqueue(const T& val) override
	{
	    // проверка на полноту
	    if (head == tail && last == ENQUEUE)
		throw std::runtime_error("Queue overflow");
	    data[tail] = val;
	    // операцией взятия остатка
	    // закольцовываем массив
	    tail = (tail + 1) % size;
	    last = ENQUEUE;
	}

	virtual T dequeue() override
	{
	    T ret = peek();
	    head = (head+1) % size;
	    last = DEQUEUE;
	    return ret;
	}

	virtual const T& peek() const override
	{
	    if (empty())
		throw std::runtime_error("Queue underflow");
	    return data[head];
	}

	virtual bool empty() const override
	{
	    return (head == tail && last == DEQUEUE);
	}
    };
}
