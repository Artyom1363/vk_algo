#include <iostream>

class Point {
public:
    int x, y;
    Point() {}
    Point(double x, double y) : x(x), y(y) {}
    void GetPoint(std::istream &input) {
        input >> x >> y;
    }
    void Show(std::ostream &output) {
        output << x << " " << y << std::endl;
    }

    
};

bool operator <(const Point &l, const Point &r) {
    return l.x == r.x ? (l.y < r.y) : l.x < r.x;
}

class CompPoints {
    Point reper;
    double getTan(const Point &p) {
        return (1.0 * (p.y - reper.y)) / (p.x - reper.x);
    }
public:
    CompPoints(Point p) : reper(p) {}
    bool operator()(const Point &l, const Point &r) {
        return getTan(l) > getTan(r);
    }
};


template<class T>
class IsLess {
public:
	bool operator()( const T& l, const T& r ) { return l < r; }
};

template <class T, class Compare = IsLess<T> > 
void InsertSort(T* arr, int size, Compare cmp = IsLess<T>()) {
    for (int i = 1; i < size; ++i) {
        for (int j = i; j - 1 >= 0 && cmp(arr[j], arr[j - 1]); --j) {
            std::swap(arr[j], arr[j - 1]);
        }
    }
}



int main() {
    int n;
    std::cin >> n;
    Point* arr = new Point[n];
    for (int i = 0; i < n; ++i) {
        arr[i].GetPoint(std::cin);
    }
    InsertSort(arr, n);
    CompPoints cmp(arr[0]);
    InsertSort(arr, n, cmp);
    for (int i = 0; i < n; ++i) {
        arr[i].Show(std::cout);
    }
    return 0;
}