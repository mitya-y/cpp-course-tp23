#include <iostream>

auto foo(int x = 10) {
  return [&]{return x;};
}

int main() {
  auto f = foo();
  std::cout << f(); // 32766
  return 0;
}
