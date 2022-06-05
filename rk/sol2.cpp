#include <iostream>
#include <cassert>
#include <sstream>

int binary_search(const int * a, int l, int r) { 
int m; 
while (l < r) { 
m = (l + r) / 2; 
if (a[m] < a[l]) { 
r = m; 
} 
else { 
++l; 
} 
} 
return l; 
}

int exponential_search(const int * a, int n) {
    int i = 1;
    while (i < n) {
        if (a[i / 2] <= a[i]) {
            i *= 2;
        }
        else {
            return i;
        }
    }
    return n - 1;
}

int search_inflection_point(int * A, int size) {
    int p = exponential_search(A, size);
    return binary_search(A, p / 2, p);
}


void run(std::istream &input, std::ostream &output) {
    int n = 0;
    std::cin >> n;
    int *A = new int[n];
    for (int i = 0; i < n; ++i) {
        std::cin >> A[i];
    }
    std::cout << search_inflection_point(A, n);
    delete []A;
}

void test() {
    {   
        std::stringstream input;
        std::stringstream output;
        input << "10 1 5 5 5 5 5 5 5 5 5\n";
        run(input, output);
        assert(output.str() == "0"); 
        std::cout << "here" << std::endl;
    }
    {   
        std::stringstream input;
        std::stringstream output;
        input << "10 5 1 5 5 5 5 5 5 5 5\n";
        run(input, output);
        assert(output.str() == "1"); 
    }
    {   
        std::stringstream input;
        std::stringstream output;
        input << "10 5 5 1 5 5 5 5 5 5 5\n";
        run(input, output);
        assert(output.str() == "2"); 
    }
    {   
        std::stringstream input;
        std::stringstream output;
        input << "10 5 5 5 1 5 5 5 5 5 5\n";
        run(input, output);
        assert(output.str() == "3"); 
    }
    {   
        std::stringstream input;
        std::stringstream output;
        input << "10 5 5 5 5 1 5 5 5 5 5";
        run(input, output);
        assert(output.str() == "4"); 
    }
    {   
        std::stringstream input;
        std::stringstream output;
        input << "10 5 5 5 5 5 1 5 5 5 5";
        run(input, output);
        assert(output.str() == "5"); 
    }
    {   
        std::stringstream input;
        std::stringstream output;
        input << "10 5 5 5 5 5 5 1 5 5 5";
        run(input, output);
        assert(output.str() == "6"); 
    }
    {   
        std::stringstream input;
        std::stringstream output;
        input << "10 5 5 5 5 5 5 5 1 5 5";
        run(input, output);
        assert(output.str() == "7"); 
    }

}

int main() {
    // run(cin, cout);
    test();
    return 0;
}