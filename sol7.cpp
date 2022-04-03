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


template <class T>
struct dataLaunch{
    dataLaunch() : array(NULL), size(0) {}
    dataLaunch(T* initArray, int initSize) : array(initArray), size(initSize) {}
    T* array;
    int size;
};

template <class T>
class Stack{
public:
    Stack() : size(0), allocSize(0), array(NULL) {}
    ~Stack();
    bool IsEmpty() {return size == 0;}
    void Push(T& value);
    T Pop();

private:
    int size;
    int allocSize;
    T* array;
};

template <class T>
void Stack<T>::Push(T& value) {
    if (allocSize == 0) {
        array = new T[1];
        allocSize = 1;
        size = 1;
        array[0] = value;
        return;
    }
    if (allocSize == size) {
        allocSize *= 2;
        T* arrayTmp = new T[allocSize];
        for (int i = 0; i < size; ++i) arrayTmp[i] = array[i];
        delete[] array;
        arrayTmp[size++] = value;
        array = arrayTmp;
        return;
    }
    if (size < allocSize) {
        array[size++] = value;
    }
}
template <class T>
T Stack<T>::Pop() {
    assert(!IsEmpty());
    return array[--size];
}

template <class T>
Stack<T>::~Stack() {
    if (array != NULL) {
        delete[] array;
    }
};

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

void quickSort(unsigned long long* array, int size) {
    Stack <dataLaunch <unsigned long long> > launches;
    dataLaunch <unsigned long long> launch(array, size);
    launches.Push(launch);
    while (!launches.IsEmpty()) {
        dataLaunch <unsigned long long> launch = launches.Pop();
        int pivotInd = partition(launch.array, launch.size, [](unsigned long long f1, unsigned long long f2) { return f1 < f2; });
        if (pivotInd > 1) {
            dataLaunch <unsigned long long> newLaunch(launch.array, pivotInd);
            launches.Push(newLaunch);
        }
        if (pivotInd < launch.size) {
            dataLaunch <unsigned long long> newLaunch(launch.array + pivotInd + 1, launch.size - (pivotInd + 1));
            launches.Push(newLaunch);
        }
    }
}

void run(std::istream &input, std::ostream &output) {
    int n;
    input >> n; 
    unsigned long long* a = new unsigned long long[n];
    for (int i = 0; i < n; ++i) input >> a[i];
    quickSort(a, n);
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
