#include <iostream>

int findInd(int* arr, int size) {
    int l = 0;
    int r = size;
    while (r - l > 1) {
        int mid = l + (r - l) / 2;
        if (arr[mid] > arr[l]) {
            r = mid;
        } else {
            l = mid;
        }
    }
    return l;
}

int main() {
    int n;
    std::cin >> n;
    int *arr = new int[n];
    for (int i = 0; i < n; ++i) std::cin >> arr[i];
    std::cout << findInd(arr, n);
    return 0;
}