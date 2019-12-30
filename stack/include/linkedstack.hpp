#pragma once

#include <stdexcept>
#include "interface.hpp"


namespace pandora_ds
{
    /* Стек с "применением динамической памяти" */
    template <class T>
    class LinkedStack : public IStack<T>
    {
    private:
	/* Узел стека */
	template <class U>
	struct LinkedStackNode
	{
	    // данные
	    U data;
	    // след. узел стека
	    LinkedStackNode<U>* next;

	    LinkedStackNode(U data, LinkedStackNode<U>* next)
		: data(data), next(next)
	    {}
	};

	using Node = LinkedStackNode<T>;
        Node* head;
    public:
	LinkedStack()
	    : head(nullptr)
	{}

	LinkedStack(const LinkedStack& rhs) = delete;
	LinkedStack(LinkedStack&&) = delete;
	
	virtual ~LinkedStack() override
	{
	    /* удаляем эл-ты стека, извлекая их из стека */
	    while (!empty())
		pop();
	}

	LinkedStack& operator=(const LinkedStack& rhs) = delete;
	LinkedStack& operator=(LinkedStack&& rhs) = delete;
	
	virtual void push(const T& val) override
	{
	    Node* val_node = new Node(val, head);
	    head = val_node;
	}

	virtual T pop() override
	{
	    T ret = peek();
	    Node* new_head = head->next;
	    delete head;
	    head = new_head;
	    return ret;
	}

	virtual const T& peek() const override
	{
	    if (empty())
		throw std::runtime_error("Stack undeflow");
	    return head->data;
	}

	virtual bool empty() const override
	{
	    return head == nullptr;
	}
    };
}
