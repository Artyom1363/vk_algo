/*
Подсчитать кол-во единичных бит в входном числе , 
стоящих на четных позициях. Позиции битов нумеруются с 0.
*/

#include <iostream>

int calculateOnes(int n) {
	int result = 0;
	while (n) {
		result += (n & 1);
		n >>= 2;
	}
	return result;
}

int main() {
	int n = 0;
	std::cin >> n;
	std::cout << calculateOnes(n) << "\n";
	return 0;
}