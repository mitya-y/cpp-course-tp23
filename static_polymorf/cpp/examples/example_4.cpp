#include <iostream>

template<typename T>
class Printer {
public:
    static void print(T value) {
        std::cout << "Generic: " << value << "\n";
    }
};

template<>
class Printer<int> {
public:
    static void print(int value) {
        std::cout << "Int: " << value * 2 << "\n";
    }
};

int main() {
    Printer<int>::print(5);       // Int: 10
    Printer<double>::print(3.14); // Generic: 3.14
}