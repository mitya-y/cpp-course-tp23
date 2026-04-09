#include <iostream>
#include <type_traits>

template<typename T>
void process(T value) {
    if constexpr (std::is_integral_v<T>) {
        std::cout << "Целочисленный тип: " << value * 2 << '\n';
    } else if constexpr (std::is_floating_point_v<T>) {
        std::cout << "Число с плавающей точкой: " << value / 2 << '\n';
    } else {
        std::cout << "Другой тип: " << value << '\n';
    }
}

int main() {
    process(10);     // Целочисленный тип: 20
    process(3.14);   // Число с плавающей точкой: 1.57
    process("Hi");   // Другой тип: Hi
}