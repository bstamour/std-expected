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

// Can we instantiate it with non-trivial types.
static_assert(std::is_default_constructible_v<bst::expected<std::string, int>>);
static_assert(std::is_default_constructible_v<bst::expected<int, std::string>>);
static_assert(std::is_default_constructible_v<bst::expected<std::string, std::string>>);

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

//------------------------------------------------------------------------------
// Default construction

TEST(ConstructorTests, DefaultConstructTest) {
    bst::expected<int, int> e1;
    EXPECT_EQ(e1.has_value(), true);

    struct X {
        int x = 42;
    };
    bst::expected<X, int> e2;
    EXPECT_EQ(e2.has_value(), true);
    EXPECT_EQ(e2->x, 42);
}

// Construction with a non-trivial type

TEST(ConstructorTests, NonTrivialDefaultConstructTest) {
  bst::expected<std::string, int> e1;
  EXPECT_EQ(e1.has_value(), true);
  EXPECT_EQ(e1->empty(), true);

  bst::expected<std::string, int> e2 = "hello";
  EXPECT_EQ(e2.has_value(), true);
  EXPECT_EQ(e2->empty(), false);
  EXPECT_EQ(e2.value(), "hello");
}

//------------------------------------------------------------------------------
// Copy construction

TEST(ConstructorTests, CopyConstructFromSameExpectedTest) {
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

    bst::expected<std::string, int> e5 = "hello";
    bst::expected<std::string, int> e6 = e5;
    EXPECT_EQ(e6.has_value(), true);
    EXPECT_EQ(e6.value() == e5.value(), true);
}

TEST(ConstructorTests, CopyConstructFromDifferentExpectedTest) {
   bst::expected<int, int> e1(42);
   bst::expected<long, int> e2(e1);
   EXPECT_EQ(e2.has_value(), true);
   EXPECT_EQ(*e2, 42);
}

TEST(ConstructorTests, CopyConstructFromValueTypeTest) {
    bst::expected<int, int> e1(42);
    EXPECT_EQ(e1.has_value(), true);
    EXPECT_EQ(e1.value(), 42);
}

//------------------------------------------------------------------------------
// Move construction

TEST(ConstructorTests, MoveConstructFromValueTypeTest) {
    bst::expected<std::unique_ptr<int>, int> e1(std::make_unique<int>(42));
    EXPECT_EQ(e1.has_value(), true);
    EXPECT_EQ((bool)e1.value(), true);
    EXPECT_EQ(**e1, 42);
}

TEST(ConstructorTests, MoveConstructFromSameExpectedTest) {
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

//------------------------------------------------------------------------------
// Construction from an unexpected

TEST(ConstructorTests, ConstructFromUnexpected) {
    int x = 42;
    bst::unexpected unex(x);

    bst::expected<int, int> ex(unex);

    EXPECT_EQ(ex.has_value(), false);
    EXPECT_EQ(ex.error(), 42);
}

//------------------------------------------------------------------------------
// Assignment

TEST(AssignmentTests, CopyAssignFromOther) {
    bst::expected<int, int> e1(42);
    bst::expected<int, int> e2(12);
    e1 = e2;
    EXPECT_EQ(e1.has_value(), true);
    EXPECT_EQ(*e1, 12);
}

TEST(AssignmentTests, CopyAssignFromUnexpected) {
    bst::expected<int, int> e1(42);
    e1 = bst::unexpected<int>(12);
    EXPECT_EQ(e1.has_value(), false);
    EXPECT_EQ(e1.error(), 12);
}

TEST(AssignmentTests, CopyAssignFromValue) {
    bst::expected<int, int> e1(42);
    int a = 100;
    e1 = a;
    EXPECT_EQ(e1.has_value(), true);
    EXPECT_EQ(*e1, 100);
}

TEST(AssignmentTests, CopyAssignFromConvertible) {
   bst::expected<int, int> e1(42);
    bst::expected<short, int> e3(5);
    e1 = e3;
    EXPECT_EQ(e1.has_value(), true);
    EXPECT_EQ(*e1, 5);
}

TEST(AssignmentTests, CopyAssignFromUserDeclared) {
   bst::expected<int, int> e1(42);
    struct X {
	int v = 100;
	operator int() const { return v; }
    } x;
    bst::expected<X, int> e4(x);
    e1 = e4;
    EXPECT_EQ(e1.has_value(), true);
    EXPECT_EQ(*e1, 100);
}

TEST(AssignmentTests, MoveAssignFromOther) {
    bst::expected<int, int> e1(42);
    bst::expected<int, int> e2(12);
    e1 = std::move(e2);
    EXPECT_EQ(e1.has_value(), true);
    EXPECT_EQ(*e1, 12);
}

TEST(AssignmentTests, MoveAssignFromUnexpected) {
    bst::expected<int, int> e1(42);
    e1 = bst::unexpected<int>(12);
    EXPECT_EQ(e1.has_value(), false);
    EXPECT_EQ(e1.error(), 12);
}

TEST(AssignmentTests, MoveAssignFromValue) {
    bst::expected<int, int> e1(42);
    e1 = 100;
    EXPECT_EQ(e1.has_value(), true);
    EXPECT_EQ(*e1, 100);
}

TEST(AssignmentTests, MoveAssignFromConvertible) {
   bst::expected<int, int> e1(42);
    e1 =     bst::expected<short, int>(5);
    EXPECT_EQ(e1.has_value(), true);
    EXPECT_EQ(*e1, 5);
}

TEST(AssignmentTests, MoveAssignFromUserDeclared) {
   bst::expected<int, int> e1(42);
    struct X {
	int v = 100;
	operator int() const { return v; }
    } x;
    bst::expected<X, int> e4(x);
    e1 = std::move(e4);
    EXPECT_EQ(e1.has_value(), true);
    EXPECT_EQ(*e1, 100);
}

//------------------------------------------------------------------------------
// Swapping

TEST(SwapTests, ActiveWithActive) {
    bst::expected<int, int> e1(42), e2(12);

    e1.swap(e2);
    EXPECT_EQ(e1.has_value(), true);
    EXPECT_EQ(e2.has_value(), true);

    EXPECT_EQ(*e1, 12);
    EXPECT_EQ(*e2, 42);
}

TEST(SwapTests, InactiveWithInactive) {
    bst::expected<int, int> e1(bst::unexpect, 1), e2(bst::unexpect, 2);

    e1.swap(e2);
    EXPECT_EQ(e1.has_value(), false);
    EXPECT_EQ(e2.has_value(), false);

    EXPECT_EQ(e1.error(), 2);
    EXPECT_EQ(e2.error(), 1);
}

TEST(SwapTests, ActiveWithInactive) {
    bst::expected<int, int> e1(1), e2(bst::unexpect, 2);

    e1.swap(e2);
    EXPECT_EQ(e1.has_value(), false);
    EXPECT_EQ(e2.has_value(), true);

    EXPECT_EQ(e1.error(), 2);
    EXPECT_EQ(*e2, 1);
}

TEST(SwapTests, InactiveWithActive) {
    bst::expected<int, int> e1(bst::unexpect, 1), e2(2);

    e1.swap(e2);
    EXPECT_EQ(e1.has_value(), true);
    EXPECT_EQ(e2.has_value(), false);

    EXPECT_EQ(*e1, 2);
    EXPECT_EQ(e2.error(), 1);
}
