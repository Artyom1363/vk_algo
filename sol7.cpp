/*

Дано множество целых чисел из [0..10^9] размера n. 
Используя алгоритм поиска k-ой порядковой статистики, требуется найти 
следующие параметры множества:
10%  перцентиль
медиана
90%  перцентиль

Требования: к дополнительной памяти: O(n). 
Среднее время работы: O(n)
Должна быть отдельно выделенная функция partition. 
Рекурсия запрещена. 
Решение должно поддерживать передачу функции сравнения снаружи.

Реализуйте стратегию выбора опорного элемента “медиана трёх”. 
Функцию Partition реализуйте методом прохода двумя итераторами от начала 
массива к концу.

*/


#include <iostream>
#include <cassert>
#include <sstream>
#include <cstring>
#include <math.h>


template<class Comp>
unsigned long long getMid(unsigned long long* a, int i1, int i2, int i3, Comp comp) {
    unsigned long long* array = new unsigned long long[3];
    array[0] = a[i1];
    array[1] = a[i2];
    array[2] = a[i3];
    int indexes[3] = {i1, i2, i3};
    for (int i = 1; i < 3; ++i) {
        for (int j = i; j > 0 && comp(array[j - 1], array[j]); --j) {
            std::swap(array[j], array[j - 1]);
            std::swap(indexes[j], indexes[j - 1]);
        }
    }
    delete[] array;
    return indexes[1];
}

template<class Comp>
int partition(unsigned long long* array, int size, Comp comp) {
    if (size < 1) {
        return 0;
    }
    int ind = getMid(array, 0, size / 2, size - 1, comp);
    // std::cout << "ind: " << ind << std::endl;
    std::swap(array[ind], array[size - 1]);
    int i = 0;
    for (int j = 0; j < size - 1; ++j) {
        if (comp(array[j], array[size - 1])) {
            std::swap(array[j], array[i++]);
        }
    }
    std::swap(array[size - 1], array[i]);
    return i;
}

void queueSort(unsigned long long* array, int size) {
    int pivotInd = partition(array, size, [](unsigned long long f1, unsigned long long f2) {
        return f1 < f2;
    });
    if (pivotInd > 1) {
        queueSort(array, pivotInd);
    }
    if (pivotInd < size) {
        queueSort(array + pivotInd + 1, size - (pivotInd + 1));
    }
}

void run(std::istream &input, std::ostream &output) {
    int n;
    input >> n; 
    unsigned long long* a = new unsigned long long[n];
    for (int i = 0; i < n; ++i) input >> a[i];
    queueSort(a, n);
    for (int i = 0; i < n; ++i) output << a[i] << " ";
    delete[] a;
}


void test() {
    {   
        std::stringstream input;
        std::stringstream output;
        input << "3 4 1000000 7 ";
        run(input, output);
        assert(output.str() == "4 7 1000000 "); 
    }
}

int main() {
    // test();
    run(std::cin, std::cout);
    return 0;
}
