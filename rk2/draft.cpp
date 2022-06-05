#include <iostream>
#include <optional>

using std::string;
using std::cout;

std::optional <string> func() {
    return std::nullopt;
}

int main() {
    std::optional <string> val;
    val = func();
    if (val) {
        cout << "have value: " << *val;
    } else {
        cout << "dont have value\n";
    }
    return 0;
}