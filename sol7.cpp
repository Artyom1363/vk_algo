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
struct dataLaunch {
    dataLaunch() : array(NULL), size(0), bitNumber(0) {}
    dataLaunch(T* initArray, int initSize, int gotBitNumber) : array(initArray), 
                                                               size(initSize), 
                                                               bitNumber(gotBitNumber){}
    T* array;
    int size;
    int bitNumber;
};

template <class T>
class Stack {
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

template <class T>
class SpecificComp {
public:
    SpecificComp(int gotBitNumber) : bitNumber(gotBitNumber) {}

    bool operator()(const T& l) {
        T one = 1;
        return (l & (one << bitNumber)) == 0;
    }

private:
    int bitNumber;
};


template<class T, class Comp>
int binaryPartition(T* array, int size, Comp comp) {
    if (size < 1) {
        return 0;
    }
    int i = 0;
    for (int j = 0; j < size; ++j) {
        if (comp(array[j])) {
            std::swap(array[j], array[i++]);
        }
    }
    return i;
}

template <class T>
void quickSort(T* array, int size, int maxBit) {
    Stack <dataLaunch <T> > launches;
    dataLaunch <T> launch(array, size, maxBit);
    launches.Push(launch);
    while (!launches.IsEmpty()) {
        dataLaunch <T> launch = launches.Pop();
        int bitNumber = launch.bitNumber;
        if (bitNumber < 0) continue;
        SpecificComp <T> comp(bitNumber);
        int pivotInd = binaryPartition(launch.array, launch.size, comp);
        if (pivotInd > 1) {
            dataLaunch <T> newLaunch(launch.array, pivotInd, bitNumber - 1);
            launches.Push(newLaunch);
        }
        if (pivotInd < launch.size) {
            dataLaunch <T> newLaunch(launch.array + pivotInd, launch.size - (pivotInd), bitNumber - 1);
            launches.Push(newLaunch);
        }
    }
}

void run(std::istream &input, std::ostream &output) {
    int n;
    input >> n; 
    unsigned long long* a = new unsigned long long[n];
    for (int i = 0; i < n; ++i) input >> a[i];
    quickSort(a, n, 63);
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
    {   
        std::stringstream input;
        std::stringstream output;
        input << "5 5 4 3 2 1";
        run(input, output);
        assert(output.str() == "1 2 3 4 5 "); 
    }
    {   
        std::stringstream input;
        std::stringstream output;
        input << "15 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1";
        run(input, output);
        assert(output.str() == "1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 "); 
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "5 18446744073709551615 18446744073709551614 18446744073709551613 1 2";
        run(input, output);
        assert(output.str() == "1 2 18446744073709551613 18446744073709551614 18446744073709551615 "); 
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "6 18446744073709551615 1 1 0 2 18446744073709551615";
        run(input, output);
        assert(output.str() == "0 1 1 2 18446744073709551615 18446744073709551615 "); 
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "6 5 4 3 2 1 9";
        run(input, output);
        assert(output.str() == "1 2 3 4 5 9 "); 
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "3 18446744073709551615 9223372036854775807 9223372036854775808";
        run(input, output);
        assert(output.str() == "9223372036854775807 9223372036854775808 18446744073709551615 "); 
    }

}

int main() {
    // test();
    run(std::cin, std::cout);
    return 0;
}
