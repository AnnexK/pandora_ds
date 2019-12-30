#include <iostream>
#include <memory>

#include "interface.hpp"
#include "arraystack.hpp"
#include "linkedstack.hpp"

using namespace pandora_ds;

int main()
{
    try
    {
	/* создаем нужные стеки
	   подставьте свой тип данных при необходимости */
	std::unique_ptr< IStack<char> > stk = std::make_unique< LinkedStack<char> >();
	// std::unique_ptr< IStack<T> > stk2 = std::make_unique< LinkedStack<T> >();
    
	/* здесь писать решение задачи */
	std::cout << "Solution Placeholder" << std::endl;
    
	/* деструкторы умных указателей вызовут деструкторы стеков,
	   которые освободят выделенную память */
    }
    catch (const std::exception& e)
    {
	// здесь обработка ошибок
	std::cerr << e.what() << std::endl;
    }
    return 0;
}
