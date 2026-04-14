#include <iostream>
#include <print>
#include <functional>

int main() {
  struct ExpectedType {
    int a;
    int &b;
    ExpectedType(int a, int &b) : a(a), b(b) {}
    auto operator()(int c) { a = -c; b = -(a + c); }
  };

  int b = 10;
  auto lambda_function = [a = 0, &b](int c) mutable {a = c; b = a + c;};
  using LambdaT = decltype(lambda_function);
  lambda_function(5);

  int c = 2;
  auto et = ExpectedType(0, c);

  std::println("a: {}, c: {}", et.a, c);
  (*(LambdaT *)&et)(7);
  std::println("a: {}, c: {}", et.a, c);

  std::println("a: {}, b: {}", *(int *)&lambda_function, b);
  (*(ExpectedType *)&lambda_function)(7);
  std::println("a: {}, b: {}", *(int *)&lambda_function, b);

  struct Derived : LambdaT {
    Derived(LambdaT &t) : LambdaT(t) {}
    // using LambdaT::operator();
  } d(lambda_function);
  d(10);

  std::println("typeid: {}", typeid(LambdaT).name());

  struct ExpectedSimpleType {
    static int func(int c) { return c; }
    int operator()(int c) { return func(c); }
    operator decltype(&func)() {return func;}
  };

  auto simple_lambda = [](int x) {return x;};
  int (*simple_lambda_ptr)(int) = simple_lambda;
  ExpectedSimpleType est;
  int (*est_ptr)(int) = est;

  using SimpleLambdaT = decltype(simple_lambda);
  std::println("{}, {}",
    (*(SimpleLambdaT *)&est)(7),
    (*(ExpectedSimpleType *)&simple_lambda)(8)); // 7, 8
}
