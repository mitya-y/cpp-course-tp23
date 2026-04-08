#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <variant>
#include <vector>
#include <utility>

// ── Слайд 1: Лямбда — объект замыкания ──────────────────────────────────────
//
// Компилятор генерирует анонимную структуру с operator().
// Без захвата структура имеет static-метод и оператор приведения к func*.
// Каждая лямбда — уникальный тип: две лямбды с одинаковой сигнатурой
// несовместимы.

void slide1_closure_internals() {
    std::cout << "=== Слайд 1: Лямбда — объект замыкания ===\n";

    // Лямбда без захвата: компилятор строит примерно так:
    //   struct __Closure {
    //       static int call(int x, int y) { return x + y; }
    //       operator int(*)(int,int)() const { return call; }
    //   };
    auto adder = [](int x, int y) { return x + y; };

    // Неявное приведение к указателю на функцию (только без захвата):
    int (*pf)(int, int) = adder;
    assert(pf(2, 3) == 5);

    // Тип лямбды уникален — переприсвоить нельзя:
    auto f = [] { return 1; };
    // f = [] { return 2; };  // ошибка: несовместимые типы

    // Unary+ («positive hack»): форсирует приведение к func*, тип становится
    // указателем — теперь можно присвоить любую лямбду с той же сигнатурой.
    auto g = +[] { return 1; };   // g: int(*)()
    g = [] { return 2; };         // ok — это просто присваивание указателей
    assert(g() == 2);

    // IIFE: немедленный вызов лямбды прямо на месте
    int sq = [](int x) { return x * x; }(5);
    assert(sq == 25);

    std::cout << "adder(2,3)=" << adder(2, 3)
              << "  pf(2,3)=" << pf(2, 3)
              << "  g()=" << g()
              << "  sq=" << sq << "\n";
}

// ── Слайд 2: Что и как захватывается ────────────────────────────────────────
//
// Захватывается ТОЛЬКО локальный нестатический контекст.
// Статические переменные и глобальные — прямой доступ, не захват.
// Их изменения всегда видны внутри лямбды даже при [=].

static int g_global = 1;

void slide2_capture_rules() {
    std::cout << "\n=== Слайд 2: Что и как захватывается ===\n";

    int x = 10;
    static int s = 2;

    // [=]: копии локальных переменных в момент создания лямбды.
    // s и g_global — прямой доступ, НЕ копируются.
    auto lam_copy = [=] { return x + s + g_global; };
    x = 99; s = 20; g_global = 10;
    // x: была 10 (копия), s и g_global — прямой доступ → обновлённые значения
    std::cout << "lam_copy()=" << lam_copy() << "\n"; // 10 + 20 + 10 = 40
    assert(lam_copy() == 40);

    x = 10; s = 2; g_global = 1; // сбросим для следующего теста

    // [&]: ссылки на локальные переменные.
    auto lam_ref = [&x] { return x + s + g_global; };
    x = 99;
    std::cout << "lam_ref()=" << lam_ref() << "\n"; // 99 + 2 + 1 = 102
    assert(lam_ref() == 102);

    // Смешанный захват: x по копии, b по ссылке
    int b = 5;
    auto lam_mix = [x, &b] { return x + b; }; // x=99 (копия), b по ссылке
    b = 50;
    std::cout << "lam_mix()=" << lam_mix() << "\n"; // 99 + 50 = 149
    assert(lam_mix() == 149);
}

// ── Слайд 3: mutable и захват с переименованием ──────────────────────────────
//
// operator() лямбды по умолчанию const: захваченные копии нельзя менять.
// mutable снимает const. Захват с переименованием даёт const-ссылку.
// C++14 init-capture позволяет move-захватывать некопируемые объекты.

void slide3_mutable_rename_move() {
    std::cout << "\n=== Слайд 3: mutable и захват с переименованием ===\n";

    // Счётчик через mutable: копия n изменяема между вызовами
    auto counter = [n = 0]() mutable { return ++n; };
    assert(counter() == 1);
    assert(counter() == 2);
    assert(counter() == 3);
    std::cout << "counter: 1 2 3 — ok\n";

    // Захват по const-ссылке через переименование (C++14):
    // синтаксиса [const& x] нет — используем as_const
    int x = 5;
    auto lmd = [&rx = std::as_const(x), vx = x] {
        // rx: const int& — изменить через rx нельзя
        return vx + rx;
    };
    assert(lmd() == 10); // 5 + 5
    x = 100;
    assert(lmd() == 105); // vx=5 (копия), rx=100 (const ref)
    std::cout << "lmd() после x=100: " << lmd() << "\n";

    // Move-захват: unique_ptr некопируем, только move-захват (C++14)
    auto ptr = std::make_unique<int>(42);
    auto f = [p = std::move(ptr)] { return *p; };
    assert(ptr == nullptr); // ptr перемещён внутрь замыкания
    assert(f() == 42);
    std::cout << "move-захват: f()=" << f() << ", ptr==nullptr\n";
}

// ── Слайд 4: Callables и std::invoke ────────────────────────────────────────
//
// Callable: лямбда, функтор, указатель на функцию, указатель на метод,
// указатель на член данных. std::invoke абстрагирует все варианты.
// «Проекция Лававея»: указатель на член — тоже callable для std::invoke.

struct Point {
    int x, y;
    int sum() const { return x + y; }
};

void slide4_invoke() {
    std::cout << "\n=== Слайд 4: Callables и std::invoke ===\n";

    Point p{3, 7};
    auto psum = &Point::sum;
    auto px   = &Point::x;

    // std::invoke: единый синтаксис для всех видов callable
    std::cout << "invoke(sum, p)=" << std::invoke(psum, p)  << "\n"; // 10
    std::cout << "invoke(sum,&p)=" << std::invoke(psum, &p) << "\n"; // 10
    std::cout << "invoke(&x, p)=" << std::invoke(px, p)    << "\n"; // 3

    assert(std::invoke(psum, p)  == 10);
    assert(std::invoke(psum, &p) == 10);
    assert(std::invoke(px, p)    == 3);

    // Обобщённый вызов через шаблон: работает с любым callable
    auto call = [](auto f, auto&&... args) {
        return std::invoke(f, std::forward<decltype(args)>(args)...);
    };
    assert(call(psum, p) == 10);
    assert(call(px,   p) == 3);

    // «Проекция Лававея»: указатель на поле ≡ callable
    std::vector<Point> pts = {{1, 10}, {2, 20}, {3, 5}};

    // Найти минимум по полю y: лямбда-способ
    auto min_lambda = *std::min_element(pts.begin(), pts.end(),
        [](const Point& a, const Point& b) { return a.y < b.y; });

    // Указатель на поле — более наглядно через ranges (C++20),
    // но и через std::invoke в алгоритме:
    auto min_proj = *std::min_element(pts.begin(), pts.end(),
        [](const Point& a, const Point& b) {
            return std::invoke(&Point::y, a) < std::invoke(&Point::y, b);
        });

    assert(min_lambda.y == 5);
    assert(min_proj.y   == 5);
    std::cout << "min by y: " << min_proj.y << "\n";
}

// ── Слайд 5: Generic lambdas (C++14 / C++20) ────────────────────────────────
//
// C++14: auto-параметр → шаблонный operator(). Одна лямбда для любых типов.
// C++20: явные шаблонные параметры + requires + concepts. Доступ к T по имени.

// Concept для C++20 части (определён на уровне файла)
template <typename T, typename U>
concept Addable = requires(T t, U u) { t + u; };

void slide5_generic_lambdas() {
    std::cout << "\n=== Слайд 5: Generic lambdas ===\n";

    // C++14: auto → шаблонный operator()
    auto adder = [](auto x, auto y) { return x + y; };
    assert(adder(1, 2)     == 3);
    assert(adder(1.5, 2.5) == 4.0);
    std::cout << "adder(1,2)=" << adder(1, 2)
              << "  adder(1.5,2.5)=" << adder(1.5, 2.5) << "\n";

    // C++14: auto в нескольких параметрах — типы независимы
    auto mix = [](auto x, auto y) { return std::to_string(x) + std::to_string(y); };
    (void)mix; // типы выводятся независимо

    // C++20: явный шаблонный параметр — T доступен по имени в теле
    auto front = []<typename T>(std::vector<T>& v) -> T& {
        return v.front();
    };
    std::vector<int> vi = {3, 1, 2};
    front(vi) = 99;
    assert(vi[0] == 99);
    std::cout << "front after assign: " << vi[0] << "\n";

    // C++20: requires — ограничение совместимости типов
    auto typed_add = []<typename T, typename U>(T x, U y)
        requires Addable<T, U> { return x + y; };
    assert(typed_add(3, 4)     == 7);
    assert(typed_add(1.0, 2.0) == 3.0);
    // typed_add("a", 1);  // ошибка: Addable<const char*, int> не выполнен
    std::cout << "typed_add(3,4)=" << typed_add(3, 4) << "\n";

    // C++20: Без явных параметров — концепт проверяется независимо для каждого
    // [](Addable auto x, Addable auto y) — x и y могут быть разных типов!
    // С явными параметрами можно связать: <typename T> requires Addable<T,T>
    auto same_type_add = []<typename T>(T x, T y)
        requires requires(T a, T b) { a + b; } { return x + y; };
    assert(same_type_add(5, 3) == 8);
    std::cout << "same_type_add(5,3)=" << same_type_add(5, 3) << "\n";
}

// ── Слайд 6: «Перегрузка» лямбд (overload trick) ────────────────────────────
//
// Лямбды нельзя перегружать напрямую. Обходной путь: наследование от лямбд
// + using раскрывает их operator(). Компилятор делает обычный overload resolution.
// C++17: fold expression для using. Deduction guide убирает make_overload.

template <typename... Fs>
struct overload : Fs... { using Fs::operator()...; };

// Deduction guide (C++17): позволяет aggregate-инициализацию без make_overload
template <typename... Fs>
overload(Fs...) -> overload<Fs...>;

void slide6_overload_variant() {
    std::cout << "\n=== Слайд 6: overload trick и std::variant ===\n";

    // Обычная «перегрузка» между лямбдами:
    auto f = overload {
        [](int i)    { std::cout << "int: "    << i    << "\n"; return 1; },
        [](double d) { std::cout << "double: " << d    << "\n"; return 2; },
        [](const std::string& s) { std::cout << "string: " << s << "\n"; return 3; },
    };

    assert(f(42)          == 1);
    assert(f(3.14)        == 2);
    assert(f(std::string("hi")) == 3);

    // std::variant + std::visit + overload ≈ паттерн-матчинг
    using V = std::variant<int, double, std::string>;

    auto describe = overload {
        [](int i)               -> std::string { return "int("    + std::to_string(i) + ")"; },
        [](double d)            -> std::string { return "double(" + std::to_string(d) + ")"; },
        [](const std::string& s)-> std::string { return "string(" + s + ")"; },
    };

    V v1 = 42;
    V v2 = 3.14;
    V v3 = std::string("hello");

    assert(std::visit(describe, v1) == "int(42)");
    assert(std::visit(describe, v3) == "string(hello)");

    std::cout << std::visit(describe, v1) << "\n";
    std::cout << std::visit(describe, v2) << "\n";
    std::cout << std::visit(describe, v3) << "\n";
}

// ── Слайд 7: std::function: type erasure и рекурсия ──────────────────────────
//
// auto: нулевой overhead, конкретный тип, компилятор может инлайнить.
// std::function: type erasure (void* + vtable), heap allocation, overhead.
// Рекурсивная лямбда: auto не работает (тип ещё не известен при захвате).
// std::function решает проблему, но нельзя перемещать — ссылка станет висячей.

void slide7_type_erasure_recursion() {
    std::cout << "\n=== Слайд 7: std::function и рекурсия ===\n";

    // auto: конкретный тип — компилятор видит всё, инлайнит
    auto fast = [x = 5](int a) { return x + a; };
    assert(fast(3) == 8);

    // std::function: стирание типа, heap indirection при каждом вызове
    std::function<int(int)> flex = [x = 5](int a) { return x + a; };
    assert(flex(3) == 8);

    // Рекурсивная лямбда — через std::function
    // auto fact = [&fact](int n){...};  // ошибка: fact ещё не объявлен
    std::function<int(int)> fact = [&fact](int n) {
        return n <= 1 ? 1 : n * fact(n - 1);
    };
    assert(fact(0) == 1);
    assert(fact(1) == 1);
    assert(fact(5) == 120);
    std::cout << "fact(5)=" << fact(5) << "\n";

    // Внимание: fact захвачена по ссылке — перемещение приведёт к UB!
    // std::function<int(int)> moved = std::move(fact);
    // moved(5);  // UB: ссылка на fact — висячая

    // std::function нужна: хранение, разные типы лямбд, рекурсия
    std::vector<std::function<int(int)>> callbacks;
    callbacks.push_back([](int x) { return x + 1; });
    callbacks.push_back([](int x) { return x * 2; });
    assert(callbacks[0](5) == 6);
    assert(callbacks[1](5) == 10);
    std::cout << "callbacks: " << callbacks[0](5) << " " << callbacks[1](5) << "\n";
}

// ── Слайд 8: Deducing this (C++23) ──────────────────────────────────────────
//
// C++23 позволяет явно аннотировать параметр this через (this auto&& self).
// Открывает: рекурсивные лямбды без std::function, forward_like для захватов,
// один метод вместо трёх перегрузок const/&/&&.

#if __cplusplus >= 202302L

struct Widget {
    std::string data;

    // Один метод вместо get() &, get() const&, get() &&
    auto get(this auto&& self) -> decltype(auto) {
        return std::forward_like<decltype(self)>(self.data);
    }
};

void slide8_deducing_this() {
    std::cout << "\n=== Слайд 8: Deducing this (C++23) ===\n";

    // Рекурсивная лямбда без std::function и без захвата:
    // self — это сам тип замыкания, вывод безопасен
    auto fact = [](this auto&& self, int n) -> int {
        return n <= 1 ? 1 : n * self(n - 1);
    };
    assert(fact(5) == 120);
    assert(fact(0) == 1);
    std::cout << "fact(5)=" << fact(5) << " (без std::function)\n";

    // forward_like: проброс с value category замыкания
    // Если self — rvalue, move(m); если lvalue — m по ссылке
    int m = 42;
    auto cb = [m](this auto&& self) -> decltype(auto) {
        return std::forward_like<decltype(self)>(m);
    };
    auto result = cb();        // lvalue: возвращает const int&
    assert(result == 42);

    // Widget: один метод get(), работает для lvalue, const lvalue, rvalue
    Widget w{"hello"};
    std::string& r1       = w.get();               // lvalue → String&
    const std::string& r2 = std::as_const(w).get(); // const& → const String&
    std::string moved     = Widget{"world"}.get();  // rvalue → String&&, moved
    assert(r1  == "hello");
    assert(r2  == "hello");
    assert(moved == "world");
    std::cout << "Widget::get: " << r1 << " " << r2 << " " << moved << "\n";
}

#else

void slide8_deducing_this() {
    std::cout << "\n=== Слайд 8: Deducing this (C++23) ===\n";
    std::cout << "Требуется компилятор с поддержкой C++23 (gcc-13+, clang-16+)\n";
    std::cout << "Скомпилируйте с: -std=c++23\n";
}

#endif // __cplusplus >= 202302L

// ── main ─────────────────────────────────────────────────────────────────────

int main() {
    slide1_closure_internals();
    slide2_capture_rules();
    slide3_mutable_rename_move();
    slide4_invoke();
    slide5_generic_lambdas();
    slide6_overload_variant();
    slide7_type_erasure_recursion();
    slide8_deducing_this();

    std::cout << "\nВсе assert прошли успешно.\n";
    return 0;
}
