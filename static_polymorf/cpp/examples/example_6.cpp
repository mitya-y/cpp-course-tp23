#include <iostream>

template <class V> requires (sizeof(V) < 32)
void wrap(V) {
    std::cout << "small type\n";
}

template <class V> requires (sizeof(V) >= 32)
void wrap(V) {
    std::cout << "large type\n";
}

int main() {
    wrap(char{});      // small type
    wrap(double{});    // small type
    struct Big { char data[32]; };
    wrap(Big{});       // large type (32 >= 32)
}