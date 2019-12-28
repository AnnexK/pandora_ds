#pragma once

#include <cstddef>

namespace pandora_ds
{
    /* Класс ISorter<T> предоставляет интерфейс,
       состоящий из метода sort,
       принимающего массив типа T и его размер
       и сортирующего переданный массив. */
    template <class T>
    class ISorter
    {
    public:
	/* конс и дестр -- по умолчанию
	   дестр можно перегрузить в подклассах */
	ISorter() = default;
	virtual ~ISorter() = default;

	/* интерфейс класса */
	virtual unsigned int sort(T *, size_t) = 0;
    };
}
