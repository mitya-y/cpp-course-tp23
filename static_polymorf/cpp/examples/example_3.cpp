#include <iostream>

template<typename T>
struct Storage {
    static void show() { std::cout << "Обычный тип\n"; }
};

// Частичная специализация для указателей
template<typename T>
struct Storage<T*> {
    static void show() { std::cout << "Указатель на " << typeid(T).name() << '\n'; }
};

int main() {
    Storage<int>::show();    // Обычный тип
    Storage<int*>::show();   // Указатель на int
}