/*
Реализовать очередь с помощью двух стеков.
Требования: Очередь должна быть реализована в виде класса. Стек тоже должен быть реализован в виде класса.

Во всех задачах из следующего списка следует написать структуру данных, обрабатывающую команды push* и pop*.
Формат входных данных.
В первой строке количество команд n. n ≤ 1000000.
Каждая команда задаётся как 2 целых числа: a b.
a = 1 - push front
a = 2 - pop front
a = 3 - push back
a = 4 - pop back
Команды добавления элемента 1 и 3 заданы с неотрицательным параметром b.
Для очереди используются команды 2 и 3. Для дека используются все четыре команды.
Если дана команда pop*, то число b - ожидаемое значение. Если команда pop вызвана для пустой структуры данных, то ожидается “-1”. 
Формат выходных данных.
Требуется напечатать YES - если все ожидаемые значения совпали. Иначе, если хотя бы одно ожидание не оправдалось, то напечатать NO.

*/

#include <iostream>
#include <cassert>
#include <sstream>


template <typename T>
class Stack {
public:
    Stack() : arr(NULL), size(0), allocSize(0) {}
    Stack& operator=(const Stack &other) = delete;
    ~Stack();

    void PushBack(const T &value);
    int Size() { return size; }
    bool IsEmpty() { return size == 0; }
    void Clear();
    T PopBack();
    void Transfer(Stack<T> &anotherStack);

private:
    T* arr;
    int size = 0;
    int allocSize = 0;
};

template <typename T>
void Stack<T>::PushBack(const T& value) {
    if (size == 0) {
        arr = new T[1];
        size = 1;
        arr[0] = value;
        allocSize = 1;
        return;
    }
    if (size == allocSize) {
        allocSize *= 2;
        T *tmp = new T[allocSize];
        for (int i = 0; i < size; ++i) tmp[i] = arr[i];
        delete[] arr;
        arr = tmp;
        arr[size] = value;
        ++size;
        return;
    }
    if (size < allocSize) {
        arr[size++] = value;
        return;
    }
    
}

template <typename T>
void Stack<T>::Clear() {
    delete[] arr;
    arr = NULL;
    size = 0;
    allocSize = 0;
}

template <typename T>
T Stack<T>::PopBack() {
    assert(!IsEmpty());
    --size;
    return arr[size];
}

template <typename T>
void Stack<T>::Transfer(Stack<T> &anotherStack) {
    assert(anotherStack.IsEmpty());
    for (int i = size - 1; i >= 0; --i) {
        anotherStack.PushBack(arr[i]);
    }
    Clear();
}

template <typename T>
Stack<T>::~Stack() {
    if (arr != NULL) {
        delete[] arr;
    }
}


template <typename T>
class Queue {
public:
    Queue() = default;
    Queue& operator=(const Queue &other) = delete;
    ~Queue() = default;

    void PushBack(const T &value);
    T PopFront();
    bool IsEmpty() { return (add.IsEmpty() && read.IsEmpty()); }

private:
    Stack <T> add;
    Stack <T> read;
};


template <typename T>
void Queue<T>::PushBack(const T &value) {
    add.PushBack(value);
}

template <typename T>
T Queue<T>::PopFront() {
    assert(!IsEmpty());
    if (read.IsEmpty()) { add.Transfer(read); }
    return read.PopBack();
}


void run(std::istream &input, std::ostream &output) {
    int n;
    input >> n;
    Queue <int> myQueue;
    bool flag = true;
    while (n--) {
        int type, value;
        input >> type >> value;
        switch (type) {
        case 2:
            if (myQueue.IsEmpty()) {
                if (value != -1) flag = false;
            } else if (!(myQueue.PopFront() == value)) {
                flag = false;
            }
            break;
        case 3:
            myQueue.PushBack(value);
            break;
        default:
            assert(false);
        }
    }
    output << (flag ? "YES" : "NO") << std::endl;
}

void testQueue() {
    {   
        std::stringstream input;
        std::stringstream output;
        input << "3 3 44 3 50 2 44";
        run(input, output);
        assert(output.str() == "YES\n"); 
    }
    {   
        std::stringstream input;
        std::stringstream output;
        input << "2 2 -1 3 10";
        run(input, output);
        assert(output.str() == "YES\n"); 
    }
    {   
        std::stringstream input;
        std::stringstream output;
        input << "2 3 44 2 66 ";
        run(input, output);
        assert(output.str() == "NO\n"); 
    }

}

int main() {
    // testQueue();
    run(std::cin, std::cout);
    return 0;
}