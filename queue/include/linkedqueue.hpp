#pragma once


#include "interface.hpp"


namespace pandora_ds
{
    /* Очередь с "применением динамической памяти" */
    template <class T>
    class LinkedQueue : public IQueue<T>
    {
    private:
	/* Узел очереди */
	template <class U>
	struct LinkedQueueNode
	{
	    U data;
	    LinkedQueueNode<U>* next;

	    LinkedQueueNode(U data, LinkedQueueNode<U>* prev)
		: data(data), next(nullptr)
	    {
		if (prev != nullptr) prev->next = this;
	    }
	};

	using Node = LinkedQueueNode<T>;
        Node* head;
	Node* tail;
    public:
	LinkedQueue()
	    : head(nullptr), tail(nullptr)
	{}

	LinkedQueue(const LinkedQueue&) = delete;
	LinkedQueue(LinkedQueue&&) = delete;

	virtual ~LinkedQueue() override
	{
	    while (!empty())
		dequeue();
	}

        LinkedQueue& operator=(const LinkedQueue&) = delete;
	LinkedQueue& operator=(LinkedQueue&) = delete;

	virtual void enqueue(const T& val) override
	{
	    Node *val_node = new Node(val, tail);
	    tail = val_node;
	    if (head == nullptr) head = val_node;
	}

	virtual T dequeue() override
	{
	    T ret = peek();
	    Node *next_head = head->next;
	    delete head;
	    head = next_head;
	    if (head == nullptr) tail = nullptr;
	    return ret;
	}

	virtual const T& peek() const override
	{
	    if (empty())
		throw std::runtime_error("Queue underflow");
	    return head->data;
	}

	virtual bool empty() const override
	{
	    return (head == nullptr);
	}
    };
}
