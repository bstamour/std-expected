#include <expected/expected.hpp>

#include <gtest/gtest.h>

#include <type_traits>

//------------------------------------------------------------------------------

//
// Static tests first. Make sure we can compile statements.
//

namespace static_tests {

struct X {
    X(int) {}
};
static_assert(!std::is_default_constructible_v<X>);

struct Y {
    Y(Y const&) = delete;
};

struct Z {
    Z() = default;
    Z(Z const&) {} // non-trivial.
    Z(Z&&) {}      // non-trivial.
};
static_assert(!std::is_trivially_copyable_v<Z>);

struct A {
    A(int) {}
};

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
static_assert(std::is_constructible_v<bst::expected<int, int>,
                                      const bst::expected<short, int>&>);
static_assert(std::is_constructible_v<bst::expected<A, int>,
                                      const bst::expected<int, int>&>);

// Move construction
static_assert(std::is_constructible_v<bst::expected<int, int>,
                                      bst::expected<int, int>&&>);
static_assert(
    std::is_constructible_v<bst::expected<Z, int>, bst::expected<Z, int>&&>);
static_assert(std::is_constructible_v<bst::expected<int, int>,
                                      bst::expected<short, int>&&>);
static_assert(
    std::is_constructible_v<bst::expected<A, int>, bst::expected<int, int>&&>);

} // namespace static_tests

//------------------------------------------------------------------------------

//
// Runtime behavioural tests.
//

TEST(HelloTest, BasicAssertions) {
    EXPECT_STRNE("hello", "world");
    EXPECT_EQ(7 * 6, 42);
}
