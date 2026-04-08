#include <iostream>
#include <ostream>

int g = 1;
void foo() {
  int x = 10; static int s = 2;

  auto lam = [=] { return x + s + g; };
  auto lam2 = [&] { return x + s + g; };
  auto lam3 = [&x, copy_s=s, copy_g=g] {
    return x + copy_s + copy_g;
  };
  x = 99;
  std::cout << lam() << std::endl; // 13
  std::cout << lam2() << std::endl; // 102
  std::cout << lam3() << std::endl; // 102
  s = 10;
  std::cout << lam() << std::endl;  // 21
  std::cout << lam2() << std::endl;  // 110
  std::cout << lam3() << std::endl;  // 102
}

int main() {
  foo();
}
