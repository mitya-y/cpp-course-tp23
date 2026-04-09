#include <iostream>

void print(int value) {
    std::cout << "int: " << value << "\n";
}

void print(double value) {
    std::cout << "double: " << value << "\n";
}

void print(const std::string& value) {
    std::cout << "string: " << value << "\n";
}

int main() {
    print(42);                      // print(int)
    print(3.14);                    // print(double)
    print(std::string("hello"));    // print(const string&)
}