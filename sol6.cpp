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

int getMid(int* a, int i1, int i2, int i3) {
    int *array = new int[3];
    array[0] = a[i1];
    array[1] = a[i2];
    array[2] = a[i3];
    int indexes[3] = {i1, i2, i3};
    for (int i = 1; i < 3; ++i) {
        for (int j = i; j > 0 && array[j] > array[j - 1]; --j) {
            std::swap(array[j], array[j - 1]);
            std::swap(indexes[j], indexes[j - 1]);
        }
    }
    delete[] array;
    return indexes[1];
}

int partition(int* array, int size) {
    if (size < 1) {
        return 0;
    }
    int ind = getMid(array, 0, size / 2, size - 1);
    // std::cout << "ind: " << ind << std::endl;
    std::swap(array[ind], array[size - 1]);
    int i = 0;
    for (int j = 0; j < size - 1; ++j) {
        if (array[j] <= array[size - 1]) {
            std::swap(array[j], array[i++]);
        }
    }
    std::swap(array[size - 1], array[i]);
    return i;
}

int findKStat(int* array, int size, int k) {
    int* arrCopy = new int[size];
    memcpy(arrCopy, array, sizeof(int) * size);
    int pivotInd = 0;
    do {
        pivotInd = partition(arrCopy, size);
        // std::cout << "pivot: " << pivotInd << std::endl;
        // std::cout << "k: " << k << std::endl;
        if (pivotInd < k) {
            arrCopy += pivotInd + 1;
            size -= pivotInd + 1;
            k -= pivotInd + 1;
        } else if (pivotInd > k) {
            size = pivotInd;
        } else {
            break;
        }
    } while (true);
    int res = arrCopy[pivotInd];
    // delete[] arrCopy;
    return res;
}

void run(std::istream &input, std::ostream &output) {
    int n;
    input >> n; 
    int* a = new int[n];
    for (int i = 0; i < n; ++i) input >> a[i];
    int k1 = n / 10;
    int k2 = n / 2;
    int k3 = ceil((n * 9) / 10);
    // std::cout << "k3: " << k3 << std::endl;
    output << findKStat(a, n, k1) << "\n" << findKStat(a, n, k2) << "\n";
    output << findKStat(a, n, k3);
    delete[] a;
}


void test() {
    {   
        std::stringstream input;
        std::stringstream output;
        input << "10 1 2 3 4 5 6 7 8 9 10 ";
        run(input, output);
        assert(output.str() == "2\n6\n10"); 
    }
    {   
        std::stringstream input;
        std::stringstream output;
        input << "10 10 9 8 7 6 5 4 3 2 1 ";
        run(input, output);
        assert(output.str() == "2\n6\n10"); 
    }
    {   
        std::stringstream input;
        std::stringstream output;
        input << "9 10 9 8 7 6 5 4 3 2 ";
        run(input, output);
        assert(output.str() == "2\n6\n10"); 
    }
    {   
        std::stringstream input;
        std::stringstream output;
        input << "15 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1";
        run(input, output);
        assert(output.str() == "2\n8\n14"); 
    }
}

int main() {
    // test();
    run(std::cin, std::cout);
    return 0;
}
