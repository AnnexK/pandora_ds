#include <iostream>
#include <memory>


#include "interface.hpp"
#include "ringqueue.hpp"
#include "linkedqueue.hpp"

using namespace pandora_ds;

int main()
{
    try
    {
	/* создаем нужные очереди
	   подставьте свой тип данных при необходимости */
	std::unique_ptr< IQueue<int> > q = std::make_unique< RingBufferQueue<int> >();
	// std::unique_ptr< IQueue<T> > q2 = std::make_unique< LinkedQueue<T> >();
    
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
