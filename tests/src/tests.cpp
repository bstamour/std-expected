#include <expected/expected.hpp>

#include <gtest/gtest.h>

#include <memory>
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

TEST(DefaultConstructTest, ConstructorTests) {
    bst::expected<int, int> e1;
    EXPECT_EQ(e1.has_value(), true);

    struct X {
        int x = 42;
    };
    bst::expected<X, int> e2;
    EXPECT_EQ(e2.has_value(), true);
    EXPECT_EQ(e2->x, 42);
}

TEST(CopyConstructFromSameExpectedTest, ConstructorTests) {
    bst::expected<int, int> e1;
    bst::expected<int, int> e2 = e1;
    EXPECT_EQ(e2.has_value(), true);

    struct X {
        int x = 42;
    };
    bst::expected<X, int> e3;
    bst::expected<X, int> e4 = e3;
    EXPECT_EQ(e4.has_value(), true);
    EXPECT_EQ(e4->x, 42);
}

TEST(CopyConstructFromValueTypeTest, ConstructorTests) {
    bst::expected<int, int> e1(42);
    EXPECT_EQ(e1.has_value(), true);
    EXPECT_EQ(e1.value(), 42);
}

TEST(MoveConstructFromValueTypeTest, ConstructorTests) {
    bst::expected<std::unique_ptr<int>, int> e1(std::make_unique<int>(42));
    EXPECT_EQ(e1.has_value(), true);
    EXPECT_EQ((bool)e1.value(), true);
    EXPECT_EQ(**e1, 42);
}

TEST(MoveConstructFromSameExpectedTest, ConstructorTests) {
    bst::expected<int, int> e1;
    bst::expected<int, int> e2 = std::move(e1);
    EXPECT_EQ(e2.has_value(), true);

    std::unique_ptr<int> data{new int(42)};

    bst::expected<decltype(data), int> e3(std::move(data));
    bst::expected<decltype(data), int> e4 = std::move(e3);

    EXPECT_EQ(e4.has_value(), true);
    EXPECT_EQ(**e4, 42);
    EXPECT_EQ(*e3, nullptr);
}
