/*
4.3 Планировщик процессов
В операционной системе Technux есть планировщик процессов. 
Каждый процесс характеризуется:
приоритетом P
временем, которое он уже отработал t
временем, которое необходимо для завершения работы процесса T 
Планировщик процессов выбирает процесс с минимальным значением P * (t + 1), выполняет его время P и кладет обратно в очередь процессов.
Если выполняется условие t >= T, то процесс считается завершенным и удаляется из очереди.
Требуется посчитать кол-во переключений процессора.
Формат входных данных:  Сначала вводится кол-во процессов. После этого процессы в формате P T
Формат выходных данных: Кол-во переключений процессора.

*/

#include <iostream>
#include <sstream>
#include <vector>
#include <cassert>
#include <cstring>

struct Process {
    int priority;
    int timePassed;
    int timeLeft;
    void UpdateAfterWork() { timePassed += priority; }
    bool IsDone() { return timePassed >= timeLeft; }
    Process() : priority(0), timePassed(0), timeLeft(0) {}
};

bool operator <(const Process& l, const Process& r) {
    return l.priority * (l.timePassed + 1) < r.priority * (r.timePassed + 1);
}

template <class T>
class IsLess {
public:
    bool operator ()(const T& l, const T& r) {
        return l < r;
    }
};

template<class T, class Compare = IsLess<T> >
class Heap {
public:
    Heap(T* array, int sizeArr, Compare comp = IsLess <T> ());
    Heap& operator=(const Heap &other) = delete;
    ~Heap();
    T ExtractMin();
    bool IsEmpty();
    void UpdateProcess();
    T& PeekMin();
    void SiftDown(int ind);

private:
    int size;
    T* arr;
    Compare cmp;
};

template<class T, class Compare>
Heap<T, Compare>::Heap(T* array, int sizeArr, Compare comp) {
    size = sizeArr;
    arr = new T[size];
    memcpy(arr, array, sizeof(T) * size);
    cmp = comp;
    for (int i = size / 2; i >= 0; --i) {
        SiftDown(i);
    }
}

template<class T, class Compare>
void Heap<T, Compare>::SiftDown(int ind) {
    int left = ind * 2 + 1;
    int right = ind * 2 + 2;
    int smallest = ind;
    if (left < size && cmp(arr[left], arr[ind])) {
        smallest = left;
    }
    if (right < size && cmp(arr[right], arr[smallest])) {
        smallest = right;
    }
    if (smallest != ind) {
        std::swap(arr[smallest], arr[ind]);
        SiftDown(smallest);
    }
}

template <class T, class Compare>
T Heap<T, Compare>::ExtractMin() {
    assert(!IsEmpty());
    T returnValue = arr[0];
    arr[0] = arr[--size];
    if (!IsEmpty()) {
        SiftDown(0);
    }
    return returnValue;
}

template <class T, class Compare>
bool Heap<T, Compare>::IsEmpty() {
    return size == 0;
}

template <class T, class Compare>
T& Heap<T, Compare>::PeekMin() {
    assert (!IsEmpty());
    return arr[0];
}

template <class T, class Compare> 
Heap<T, Compare>::~Heap() {
    delete[] arr;
}

void run(std::istream &input, std::ostream &output) {
    int n;
    input >> n;
    Process* processes = new Process[n];
    for (int i = 0; i < n; ++i) {
        input >> processes[i].priority >> processes[i].timeLeft;
    }
    Heap <Process> myHeap(processes, n);
    int count = 0;
    while (!myHeap.IsEmpty()) {
        Process& last = myHeap.PeekMin();
        last.UpdateAfterWork();
        if (last.IsDone()) {
            myHeap.ExtractMin();
        } else {
            myHeap.SiftDown(0);
        }
        ++count;
    }
    output << count << std::endl;
    delete[] processes;
}

int main() {
    run(std::cin, std::cout);
    return 0;
}