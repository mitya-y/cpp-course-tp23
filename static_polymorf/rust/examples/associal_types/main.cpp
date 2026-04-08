#include <concepts>

template<typename T>
class VecWrapper {
public:
  using Item = T;
  Item* get() { return nullptr; }
};

template<typename C>
concept Container = requires(C c) {
  typename C::Item;
  { c.get() } -> std::same_as<typename C::Item*>;
};

static_assert(Container<VecWrapper<int>>);

int main() {}
