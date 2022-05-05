/*
5.2
Группа людей называется современниками если был такой момент, когда они могли 
собраться вместе. Для этого в этот момент каждому из них должно было уже исполниться 
18 лет, но ещё не исполниться 80 лет. Дан список Жизни Великих Людей. Необходимо получить максимальное 
количество современников. В день 18летия человек уже может принимать участие в собраниях, а в день 80 летия 
и в день смерти уже не может. 
Замечание. Человек мог не дожить до 18-летия, либо умереть в день 18-летия. 
В этих случаях принимать участие в собраниях он не мог.
*/


#include <iostream>
#include <vector>
#include <sstream>
#include <cassert>
#include <algorithm>
#include <cstring>

struct Day {
    int day;
    int month;
    int year;
    bool type;
};

bool operator <(const Day& l, const Day& r) {
    return l.year == r.year ? (l.month == r.month ? (l.day == r.day ? false : (l.day < r.day)) : l.month < r.month) : (l.year < r.year);//(l.day >= r.day ? false : true));
}

bool operator ==(const Day& l, const Day& r) {
    return l.year == r.year && l.month == r.month && l.day == r.day;
}

template <class T>
class IsLess {
public:
    bool operator()(const T& l, const T& r) {
        return l < r;
    }
};

template <class T, class Compare = IsLess<T> >
void Merge(T *arr1, T *arr2, int arr1Size, int arr2Size, Compare cmp = IsLess<T>()) {
    int size = arr1Size + arr2Size;
    T *tmp = new T[size];
    int ptr1 = 0;
    int ptr2 = 0;
    int tmpPtr = 0;
    while (ptr1 < arr1Size && ptr2 < arr2Size) {
        if (cmp(arr2[ptr2], arr1[ptr1])) {
            tmp[tmpPtr] = arr2[ptr2];
            ++ptr2;
        } else {
            tmp[tmpPtr] = arr1[ptr1];
            ++ptr1;
        }
        ++tmpPtr;
    }
    while (ptr1 < arr1Size) {
        tmp[tmpPtr] = arr1[ptr1];
        ++ptr1;
        ++tmpPtr;
    }
    while (ptr2 < arr2Size) {
        tmp[tmpPtr] = arr2[ptr2];
        ++ptr2;
        ++tmpPtr;
    };
    memcpy(arr1, tmp, sizeof(T) * (size));
    delete[] tmp;
}

template <class T, class Compare = IsLess<T> >
void MergeSort(T *arr, int arrSize, Compare cmp = IsLess<T>()) {
    if (arrSize <= 1) {
        return;
    }
    int mid = arrSize / 2;
    MergeSort(arr, mid, cmp);
    MergeSort(arr + mid, arrSize - mid, cmp);
    Merge(arr, arr + mid, mid, arrSize - mid, cmp);
}


void run(std::istream &input, std::ostream &output) {
    int n;
    input >> n;
    int ptrInDaysArr = 0;
    Day* daysArr = new Day[2 * n];
    for (int i = 0; i < n; ++i) {
        input >> daysArr[ptrInDaysArr].day >> daysArr[ptrInDaysArr].month >> daysArr[ptrInDaysArr].year;
        input >> daysArr[ptrInDaysArr + 1].day >> daysArr[ptrInDaysArr + 1].month >> daysArr[ptrInDaysArr + 1].year;
        Day maxDay = daysArr[ptrInDaysArr];
        maxDay.year += 80;
        daysArr[ptrInDaysArr].year += 18;
        if (maxDay < daysArr[ptrInDaysArr + 1]) {
            daysArr[ptrInDaysArr + 1] = maxDay;
        }
        if (daysArr[ptrInDaysArr + 1] < daysArr[ptrInDaysArr] || 
            daysArr[ptrInDaysArr] == daysArr[ptrInDaysArr + 1]) {
            continue;
        }
        daysArr[ptrInDaysArr].type = true;
        daysArr[ptrInDaysArr + 1].type = false;
        ptrInDaysArr += 2;
    }
    if (ptrInDaysArr == 0) {
        output << 0;
        return;
    }
    MergeSort(daysArr, ptrInDaysArr);
    int count = 0;
    int maxi = 0;
    for (int i = 0; i < ptrInDaysArr; ++i) {
        count += (daysArr[i].type) ? 1 : -1;
        while (i + 1 < ptrInDaysArr && daysArr[i] == daysArr[i + 1]) {
            count += (daysArr[i + 1].type) ? 1 : -1;
            ++i;
        }
        maxi = std::max(maxi, count);
    }
    delete[] daysArr;
    assert(count == 0);
    output << maxi;
}


void test() {
    {   
        std::stringstream input;
        std::stringstream output;
        input << "3 2 5 1980 13 11 2055 1 1 1982 1 1 2030 2 1 1920 2 1 2000 ";
        run(input, output);
        assert(output.str() == "3"); 
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "1 1 1 2020 2 1 2038 ";
        run(input, output);
        assert(output.str() == "1");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "2 1 1 2020 2 1 2038 1 1 2002 1 1 2100";
        run(input, output);
        assert(output.str() == "2");
    }
}

int main() {
    // test();
    run(std::cin, std::cout);
    return 0;
}