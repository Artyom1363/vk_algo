#include <iostream>



int main() {
    for (int i = 0; i < 10; ++i) {
        std::cout << 10 << " ";
        for (int j = 0; j < 10; ++j) {
            if (i == j) std::cout << 1 << " ";
            else std::cout << 5 << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}