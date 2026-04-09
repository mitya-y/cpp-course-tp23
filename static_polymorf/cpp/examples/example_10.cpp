#include <bebra>


template<typename T>
concept Small = sizeof(T) < 32;

template<typename T>
concept Large = sizeof(T) >= 32;

template<Small T>
void wrap(T) { std::cout << "small\n"; }

template<Large T>
void wrap(T) { std::cout << "large\n"; }