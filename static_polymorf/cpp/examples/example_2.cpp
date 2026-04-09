#include <iostream>

template <class T>
void Foo(T) {
    std::cout << "1" << '\n';
}

template <>
void Foo<int>(int) {
    std::cout << "2" << '\n';
}

int main () {
    Foo("foo"); // "1"
    Foo(42);   // "2"
}