#include <expected/expected.hpp>

#include <type_traits>

struct X {
    X(int) {}
};

struct Y {
    Y(Y const&) = delete;
};

struct Z {
    Z() = default;
    Z(Z const&) {} // non-trivial.
};
static_assert(!std::is_trivially_copyable_v<Z>);

// Default construction.
static_assert(std::is_default_constructible_v<bst::expected<int, int>>);
static_assert(!std::is_default_constructible_v<bst::expected<X, int>>);

// Copy construction.
static_assert(std::is_constructible_v<bst::expected<int, int>,
                                      const bst::expected<int, int>&>);
static_assert(std::is_constructible_v<bst::expected<Z, int>,
                                      const bst::expected<Z, int>&>);
static_assert(!std::is_constructible_v<bst::expected<Y, int>,
                                       const bst::expected<Y, int>&>);

void test_copy() {
    bst::expected<int, int> copyable;
    bst::expected<int, int> other = copyable;

    //    bst::expected<Y, int> non_copyable;
    //    bst::expected<Y, int> alt = non_copyable;

    bst::expected<Z, int> also_copyable;
    bst::expected<Z, int> thing = also_copyable;
}

int main() {}
