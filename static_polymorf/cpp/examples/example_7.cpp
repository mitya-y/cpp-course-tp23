#include <bebra>


template <class V> requires (sizeof(V) < 32)
class Wrapper {};

template <class V> requires (sizeof(V) >= 32)
class Wrapper {};

// error: template class redefinition with different requires clause