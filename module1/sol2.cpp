/*
2_3. Даны два массива неповторяющихся целых чисел, упорядоченные
по возрастанию. A[0..n-1] и B[0..m-1]. n >> m. Найдите их пересечение.
Требования: Время работы: O(m * log k), где k - позиция элемента B[m-1]
в массиве A.. В процессе поиска очередного элемента B[i] в массиве A
пользуйтесь результатом поиска элемента B[i-1]. Внимание! В этой задаче
для каждого B[i] сначала нужно определить диапазон для бинарного поиска
размером порядка k с помощью экспоненциального поиска, а потом уже в
нем делать бинарный поиск.
*/

#include <iostream>

// this struct uses to return data
struct Node {
    int value;
    Node *next = NULL;
    Node(int val) : value(val), next(NULL) {}
};

int expFind(int sizeOfA, const int arrA[], int value) {
    int ptr = 1;
    while (ptr < sizeOfA && arrA[ptr] <= value) {
        ptr *= 2;
    }
    return ptr > sizeOfA ? sizeOfA : ptr;
}

Node *findIntersection(int sizeOfA, const int arrA[], int sizeOfB,
                       const int arrB[]) {

    Node *start = NULL;
    Node *tail = NULL;

    for (int ind = 0; ind < sizeOfB; ++ind) {
        int left = 0;
        int right = expFind(sizeOfA, arrA, arrB[ind]);
        while (right - left > 1) {
            int mid = left + (right - left) / 2;
            if (arrA[mid] > arrB[ind]) {
                right = mid;
            } else {
                left = mid;
            }
        }
        if (left < sizeOfA && arrA[left] == arrB[ind]) {
            if (start == NULL) {
                start = new Node(arrB[ind]);
                tail = start;
            } else {
                Node *newNode = new Node(arrB[ind]);
                tail->next = newNode;
                tail = tail->next;
            }
        }
    }
    return start;
}

int main() {
    int sizeOfA = 0;
    int sizeOfB = 0;
    std::cin >> sizeOfA >> sizeOfB;
    int *arrA = new int[sizeOfA], *arrB = new int[sizeOfB];
    for (int ind = 0; ind < sizeOfA; ++ind)
        std::cin >> arrA[ind];
    for (int ind = 0; ind < sizeOfB; ++ind)
        std::cin >> arrB[ind];
    Node *ptr = findIntersection(sizeOfA, arrA, sizeOfB, arrB);
    while (ptr != NULL) {
        std::cout << ptr->value << " ";
        Node *tmp = ptr;
        ptr = ptr->next;
        delete tmp;
    }
    delete[] arrA;
    delete[] arrB;
    return 0;
}