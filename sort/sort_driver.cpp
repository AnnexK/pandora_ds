#include <cstddef>
#include <iostream>
#include <fstream>

/* в теории можно не включать интерфейс
   и использовать необходимый класс... */
#include "interface.hpp"

/* классы сортировщиков тут */
#include "bubblesort.hpp"
#include "mergesort.hpp"
#include "heapsort.hpp"

using namespace pandora_ds;

int main()
{
    /* имена файлов */
    std::string choices[3] = {"file100.txt", "file1k.txt", "file10k.txt"};
    size_t choice;

    std::cout << "Choose task:\n1. 100 num\n2. 1K num\n3. 10K num\n";
    do
    {
	std::cin >> choice;
	if (choice < 1 || choice > 3)
	    std::cout << "Try again\n" << std::endl;
	else break;
    } while (true);

    size_t size = 10;
    for (size_t i = 0; i < choice; ++i)
    {
	size *= 10;
    }

    double *arr = new double[size];

    /* читаем из файла */
    std::ifstream ifp(choices[choice-1]);
    for (size_t i = 0; i < size; i++)
    {
	ifp >> arr[i];
    }

    ifp.close();

    /* вместо Heapsort подставить имя класса сортировщика
       (возможно, нужно будет также поменять арг. шаблона) */
    ISorter<double> *sorter = new Heapsort<double>();
    unsigned int cmp = sorter->sort(arr, size);
    delete sorter;

    std::ofstream ofp("result.txt");
    for (size_t i = 0; i < size; i++)
    {
	ofp << arr[i] << ' ';
    }

    ofp.close();

    /* кол-во сравнений на экране */
    std::cout << "Cmp amount: " << cmp << std::endl;
    delete[] arr;

    return 0;
}
