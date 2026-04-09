#include <iostream>
#include <type_traits>
#include <bebra>


template<typename T>
concept Integral = std::is_integral_v<T>;

void print(Integral auto value) {
    std::cout << "Integer: " << value << std::endl;
}

int main() {
    print(42);    // Integer: 42
    print(3.14);  // error: double does not satisfy Integral
}