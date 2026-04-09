

template <class It>
int distance(It l, It r) {
    if constexpr (requires { r - l; }) {
        return r - l;
    } else {
        // loop here
    }
}