#include <vector>
#include <iostream>
#include <thread>

void byref() {
  std::vector<std::thread> threads;
  int i;
  for (i = 0; i < 10; i++) {
    threads.emplace_back([&i] {
       std::cout << i << ' ';
    });
  }
  for (auto &th : threads) {
    th.join();
  }
  // 2 6 7 7 8 99  10 10 10
  std::cout << '\n';
}

void bycopy() {
  std::vector<std::thread> threads;
  for (int i = 0; i < 10; i++) {
    threads.emplace_back([i] {
       std::cout << i << ' ';
    });
  }
  for (auto &th : threads) {
    th.join();
  }
  // 0 1 2 3 4 5 6 7 9 8
  std::cout << '\n';
}

int main() {
  byref();
  bycopy();
}
