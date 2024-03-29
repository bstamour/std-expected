#ifndef BST_EXPECTED_HPP_
#define BST_EXPECTED_HPP_

// #define BST_EXPECTED_MONADIC_INTERFACE

//
// An implementation of std::expected from the upcomming C++23
//

/*
Overview
========

namespace bst {

template <class E>
class unexpected {
public:
    constexpr unexpected(const unexpected&);
    constexpr unexpected(unexpected&&);
    template <class... Args>
        constexpr explicit unexpected(std::in_place_t, Args&&...);
    template <class U, class... Args>
        constexpr explicit
        unexpected(std::in_place_t, std::initializer_list<U>, Args&&);
    template <class Err = E>
        constexpr explicit unexpected(Err&&);

    constexpr unexpected& operator=(const unexpected&) = default;
    constexpr unexpected& operator=(unexpected&&) = default;

    constexpr const E& error() const& noexcept;
    constexpr E& error() & noexcept;
    constexpr const E&& error() const&& noexcept;
    constexpr E&& error() && noexcept;

    constexpr void swap(unexpected& other) noexcept(conditional);

    template <class E2>
        friend constexpr bool
        operator==(const unexpected&, const unexpected<E2>&)

    friend constexpr void
        swap(unexpected&, unexpected&) noexcept(conditional);
};

template <class E>
class bad_expected_access : public bad_expected_access<void> {
public:
    explicit bad_expected_access(E);

    virtual const char* what() const noexcept override;

    E& error() & noexcept;
    const E& error() const& noexcept;
    E&& error() && noexcept;
    const E&& error() const&& noexcept;
};

template <>
class bad_expected_access<void> : public std::exception {
protected:
    bad_expected_access() noexcept;
    bad_expected_access(const bad_expected_access&);
    bad_expected_access(bad_expected_access&&);
    bad_expected_access& operator=(const bad_expected_access&);
    bad_expected_access& operator=(bad_expected_access&&);
    ~bad_expected_access();

public:
    virtual const char* what() const noexcept override;
};

struct unexpect_t {
    explicit unexpect_t() = default;
};
inline constexpr unexpect_t unexpect{};

template <class T, class E>
class expected {
public:
    using value_type = T;
    using error_type = E;
    using unexpected_type = unexpected<E>;

    template <class U>
        using rebind = expected<U, error_type>;

    constexpr expected();
    constexpr expected(const expected& rhs);
    constexpr expected(expected&& rhs) noexcept(conditional);
    template <class U, class G>
        constexpr explicit(conditional) expected(const expected<U, G>&);
    template <class U, class G>
        constexpr explicit(conditional) expected(expected<U, G>&&);
    template <class U = T>
        constexpr explicit(conditional) expected(U&&);
    template <class G>
        constexpr explicit(conditional) expected(const unexpected<G>&);
    template <class G>
        constexpr explicit(conditional) expected(unexpected<G>&&);
    template <class... Args>
        constexpr explicit expected(std::in_place_t, Args&&...);
    template <class U, class... Args>
        constexpr explicit
        expected(std::in_place_t, std::initializer_list<U>, Args&&...);
    template <class... Args>
        constexpr explicit expected(unexpect_t, Args&&...);
    template <class U, class... Args>
        constexpr explicit
        expected(unexpect_t, std::initializer_list<U>, Args&&...);

    constexpr ~expected();

    constexpr expected& operator=(const expected& rhs);
    constexpr expected& operator=(expected&& rhs) noexcept(conditional);
    template <class U = T>
        constexpr expected& operator=(U&& v);
    template <class G>
        constexpr expected& operator=(const unexpected<G>& e);
    template <class G>
        constexpr expected& operator=(unexpected<G>&& e);

    template <class... Args>
        constexpr T& emplace(Args&&...) noexcept;
    template <class U, class... Args>
        constexpr T& emplace(std::initializer_list<U>, Args&&...) noexcept;

    constexpr void swap(expected& rhs) noexcept(conditional);

    constexpr const T* operator->() const noexcept;
    constexpr T* operator->() noexcept;

    constexpr const T& operator*() const& noexcept;
    constexpr T& operator*() & noexcept;
    constexpr const T&& operator*() const&& noexcept;
    constexpr T&& operator*() && noexcept;

    constexpr explicit operator bool() const noexcept;
    constexpr bool has_value() const noexcept;

    constexpr const T& value() const&;
    constexpr T& value() &;
    constexpr const T&& value() const&&;
    constexpr T&& value() &&;

    constexpr const E& error() const&;
    constexpr E& error() &;
    constexpr const E&& error() const&&;
    constexpr E&& error() &&;

    template <class U>
        constexpr T value_or(U&& v) const&;
    template <class U>
        constexpr T value_or(U&& v) &&;

    template <class T2, class E2>
        friend constexpr bool
        operator==(const expected&, const expected<T2, E2>&);
    template <class T2>
        friend constexpr bool operator==(const expected&, const T2&);
    template <class E2>
        friend constexpr bool
        operator==(const expected&, const unexpected<E2>&);

    friend constexpr void
        swap(expected&, expected&) noexcept(conditional)
};

template <class E>
class expected<void, E> {
public:
    using value_type = void;
    using error_type = E;
    using unexpected_type = unexpected<E>;

    template <class U>
        using rebind = expected<U, error_type>;

    constexpr expected() noexcept;
    constexpr expected(const expected&);
    constexpr expected(expected&& rhs) noexcept(conditional);
    template <class U, class G>
        constexpr explicit(conditional) expected(const expected<U, G>&);
    template <class U, class G>
        constexpr explicit(conditional) expected(expected<U, G>&&);
    template <class G>
        constexpr explicit(conditional) expected(const unexpected<G>&);
    template <class G, class GF = G>
        constexpr explicit(conditional) expected(unexpected<G>&&);
    constexpr explicit expected(std::in_place_t) noexcept;
    template <class... Args>
        constexpr explicit expected(unexpect_t, Args&&...);
    template <class U, class... Args>
        constexpr explicit
        expected(unexpect_t, std::initializer_list<U>, Args&&...);

    constexpr ~expected();

    constexpr expected& operator=(const expected&);
    constexpr expected& operator=(expected&&) noexcept(conditional);
    template <class G>
        constexpr expected& operator=(const unexpected<G>&);
    template <class G>
        constexpr expected& operator=(unexpected<G>&&);

    constexpr void emplace() noexcept;

    constexpr void swap(expected&) noexcept(conditional);

    constexpr explicit operator bool() const noexcept;
    constexpr bool has_value() const noexcept;
    constexpr void operator*() const noexcept;
    constexpr void value() const&;
    constexpr void value() const&&;

    constexpr const E& error() const&;
    constexpr E& error() &;
    constexpr const E&& error() const&&;
    constexpr E&& error() &&;

    template <class T2, class E2>
        friend constexpr bool
        operator==(const expected&, const expected<T2, E2>&);
    template <class E2>
        friend constexpr bool
        operator==(const expected&, const unexpected<E2>&);

    friend constexpr void
        swap(expected&, expected&) noexcept(conditional)
};

} // namespace bst

*/


#include <exception>
#include <initializer_list>
#include <memory>
#include <type_traits>
#include <utility>


namespace bst {

template <class E>
class unexpected;

template <class E>
class bad_expected_access;
template <>
class bad_expected_access<void>;

struct unexpect_t {
    explicit unexpect_t() = default;
};
inline constexpr unexpect_t unexpect{};

template <class T, class E>
class expected;

template <class E>
class expected<void, E>;



namespace detail {
template <typename T, template <class> class TT>
struct is_specialization_of : std::false_type {};

template <template <class> class TT, class... Args>
struct is_specialization_of<TT<Args...>, TT> : std::true_type {};
} // namespace detail



//
// class unexpected<E>
//

template <class E>
class unexpected {
public:
    static_assert(std::is_object_v<E>, "E must be an object type");
    static_assert(!std::is_array_v<E>, "E must not be an array type");
    static_assert(!detail::is_specialization_of<E, unexpected>::value,
                  "E must not be a specialization of unexpected");
    static_assert(!std::is_const_v<E> && !std::is_volatile_v<E>,
                  "E must not be cv-qualified");

    constexpr unexpected(const unexpected&) = default;
    constexpr unexpected(unexpected&&) = default;

    template <class... Args>
        requires std::is_constructible_v<E, Args...>
    constexpr explicit unexpected(std::in_place_t, Args&&... args)
        : val_(std::forward<Args>(args)...) {}

    template <class U, class... Args>
        requires(std::is_constructible_v<E, std::initializer_list<U>&, Args...>)
    constexpr explicit unexpected(std::in_place_t, std::initializer_list<U>,
                                  Args&&... args)
        : val_(std::forward<Args>(args)...) {}

    template <class Err = E>
        requires(!std::is_same_v<std::remove_cvref_t<Err>, unexpected> &&
                 !std::is_same_v<std::remove_cvref_t<Err>, std::in_place_t> &&
                 std::is_constructible_v<E, Err>)
    constexpr explicit unexpected(Err&& e) : val_(std::forward<Err>(e)) {}

    constexpr unexpected& operator=(const unexpected&) = default;
    constexpr unexpected& operator=(unexpected&&) = default;

    constexpr const E& error() const& noexcept { return val_; }
    constexpr E& error() & noexcept { return val_; }
    constexpr const E&& error() const&& noexcept { return std::move(val_); }
    constexpr E&& error() && noexcept { return std::move(val_); }

    constexpr void
    swap(unexpected& other) noexcept(std::is_nothrow_swappable_v<E>) {
        using std::swap;
        swap(val_, other.val_);
    }

    template <class E2>
    friend constexpr bool operator==(const unexpected& lhs,
                                     const unexpected<E2>& rhs) {
        return lhs.error() == rhs.error();
    }

    friend constexpr void swap(unexpected& x,
                               unexpected& y) noexcept(noexcept(x.swap(y))) {
        x.swap(y);
    }

private:
    E val_;
};

template <class E>
unexpected(E&&) -> unexpected<std::remove_cvref_t<E>>;



//
// class bad_exception_access<void>
//

template <>
class bad_expected_access<void> : public std::exception {
protected:
    bad_expected_access() noexcept {}
    bad_expected_access(const bad_expected_access&) {}
    bad_expected_access(bad_expected_access&&) {}
    bad_expected_access& operator=(const bad_expected_access&) { return *this; }
    bad_expected_access& operator=(bad_expected_access&&) { return *this; }
    ~bad_expected_access() {}

public:
    const char* what() const noexcept override {
        return "bad void expected access";
    }
};

//
// class bad_exception_access<E>
//

template <class E>
class bad_expected_access : public bad_expected_access<void> {
public:
    explicit bad_expected_access(E e) : val_(std::move(e)) {}

    const char* what() const noexcept override { return "bad expected access"; }

    E& error() & noexcept { return val_; }
    const E& error() const& noexcept { return val_; }
    E&& error() && noexcept { return std::move(val_); }
    const E&& error() const&& noexcept { return std::move(val_); }

private:
    E val_;
};



//
// class expected<T, E>
//

template <class T, class E>
class expected {
public:
    static_assert(!std::is_reference_v<T>, "T cannot be a reference type");
    static_assert(!std::is_function_v<T>, "T cannot be a function type");
    static_assert(!std::is_same_v<std::in_place_t, std::remove_cv_t<T>>,
                  "T cannot be std::in_place_t");
    static_assert(!std::is_same_v<unexpect_t, std::remove_cv_t<T>>,
                  "T cannot be unexpect_t");
    static_assert(!detail::is_specialization_of<T, unexpected>::value,
                  "T must not be a specialization of unexpected");
    static_assert(std::is_constructible_v<unexpected<E>, E&>,
                  "Must be able to construct an unexpected<E>");

    using value_type = T;
    using error_type = E;
    using unexpected_type = unexpected<E>;

    template <class U>
    using rebind = expected<U, error_type>;

    //
    // Default constructor
    //

    constexpr expected()
        requires std::is_default_constructible_v<T>
    : val_(), has_val_(true) {}

    //
    // Copy constructor
    //

    constexpr expected(const expected& rhs)
        requires std::conjunction_v<
            std::is_copy_constructible<T>,
            std::is_copy_constructible<E>,
            std::disjunction<
                std::negation<std::is_trivially_copy_constructible<T>>,
                std::negation<std::is_trivially_copy_constructible<E>>>>
		: invalid_{}, has_val_(rhs.has_value()) {
        if (rhs.has_value())
            std::construct_at(std::addressof(val_), *rhs);
        else
            std::construct_at(std::addressof(unex_), rhs.error());
    }

    constexpr expected(const expected& rhs)
        requires(!std::is_copy_constructible_v<T> ||
                 !std::is_copy_constructible_v<E>)
    = delete;

    constexpr expected(const expected&)
        requires(std::is_trivially_copy_constructible_v<T> &&
                 std::is_trivially_copy_constructible_v<E>)
    = default;

    //
    // Move constructor
    //

    constexpr expected(expected&& rhs) noexcept(
        std::is_nothrow_move_constructible_v<T>&&
            std::is_nothrow_move_constructible_v<E>)
        requires std::conjunction_v<
            std::is_move_constructible<T>,
            std::is_move_constructible<E>,
            std::disjunction<
                std::negation<std::is_trivially_move_constructible<T>>,
                std::negation<std::is_trivially_move_constructible<E>>>>
		: invalid_{}, has_val_(rhs.has_val_) {
        if (rhs.has_value())
            std::construct_at(std::addressof(val_), std::move(*rhs));
        else
            std::construct_at(std::addressof(unex_), std::move(rhs.error()));
    }

    constexpr expected(expected&&)
        requires(std::is_trivially_move_constructible_v<T> &&
                 std::is_trivially_move_constructible_v<E>)
    = default;

    //
    // Converting constructor
    //

    template <class U, class G, class UF = const U&, class GF = const G&>
        requires(std::conjunction_v<
                 std::is_constructible<T, UF>, std::is_constructible<E, GF>,
                 std::negation<std::is_constructible<T, expected<U, G>&>>,
                 std::negation<std::is_constructible<T, expected<U, G>>>,
                 std::negation<std::is_constructible<T, const expected<U, G>&>>,
                 std::negation<std::is_constructible<T, const expected<U, G>>>,
                 std::negation<std::is_convertible<expected<U, G>&, T>>,
                 std::negation<std::is_convertible<expected<U, G> &&, T>>,
                 std::negation<std::is_convertible<const expected<U, G>&, T>>,
                 std::negation<std::is_convertible<const expected<U, G> &&, T>>,
                 std::negation<
                     std::is_constructible<unexpected<E>, expected<U, G>&>>,
                 std::negation<
                     std::is_constructible<unexpected<E>, expected<U, G>>>,
                 std::negation<std::is_constructible<unexpected<E>,
                                                     const expected<U, G>&>>,
                 std::negation<std::is_constructible<unexpected<E>,
                                                     const expected<U, G>>>>)
    constexpr explicit(!std::is_convertible_v<const U&, T> ||
                       !std::is_convertible_v<const G&, E>)
        expected(const expected<U, G>& rhs)
        : invalid_{}, has_val_(rhs.has_value()) {
        if (has_val_)
            std::construct_at(std::addressof(val_),
                              std::forward<const U&>(*rhs));
        else
            std::construct_at(std::addressof(unex_),
                              std::forward<const G&>(rhs.error()));
    }

    template <class U, class G, class UF = U, class GF = G>
        requires(std::conjunction_v<
                 std::is_constructible<T, UF>, std::is_constructible<E, GF>,
                 std::negation<std::is_constructible<T, expected<U, G>&>>,
                 std::negation<std::is_constructible<T, expected<U, G>>>,
                 std::negation<std::is_constructible<T, const expected<U, G>&>>,
                 std::negation<std::is_constructible<T, const expected<U, G>>>,
                 std::negation<std::is_convertible<expected<U, G>&, T>>,
                 std::negation<std::is_convertible<expected<U, G> &&, T>>,
                 std::negation<std::is_convertible<const expected<U, G>&, T>>,
                 std::negation<std::is_convertible<const expected<U, G> &&, T>>,
                 std::negation<
                     std::is_constructible<unexpected<E>, expected<U, G>&>>,
                 std::negation<
                     std::is_constructible<unexpected<E>, expected<U, G>>>,
                 std::negation<std::is_constructible<unexpected<E>,
                                                     const expected<U, G>&>>,
                 std::negation<std::is_constructible<unexpected<E>,
                                                     const expected<U, G>>>>)
    constexpr explicit(!std::is_convertible_v<U, T> ||
                       !std::is_convertible_v<G, E>)
        expected(expected<U, G>&& rhs)
        : invalid_{}, has_val_(rhs.has_value()) {
        if (has_val_)
            std::construct_at(std::addressof(val_), std::forward<U>(*rhs));
        else
            std::construct_at(std::addressof(unex_),
                              std::forward<G>(rhs.error()));
    }

    //
    // Converting constructor from value
    //

    template <class U = T>
        requires(!std::is_same_v<std::remove_cvref_t<U>, std::in_place_t> &&
                 !std::is_same_v<expected<T, E>, std::remove_cvref_t<U>> &&
                 std::is_constructible_v<T, U>)
    constexpr explicit(!std::is_convertible_v<U, T>) expected(U&& v)
        : val_(std::forward<U>(v)), has_val_(true) {}

    //
    // Construct from unexpected
    //

    template <class G>
        requires std::is_constructible_v<E, const G&>
    constexpr explicit(!std::is_convertible_v<const G&, E>)
        expected(const unexpected<G>& e)
        : unex_(std::forward<const G&>(e.error())), has_val_(false) {}

    template <class G>
        requires std::is_constructible_v<E, G>
    constexpr explicit(!std::is_convertible_v<G, E>) expected(unexpected<G>&& e)
        : unex_(std::forward<G>(e.error())), has_val_(false) {}

    //
    // In-place construct expected value
    //

    template <class... Args>
        requires std::is_constructible_v<T, Args...>
    constexpr explicit expected(std::in_place_t, Args&&... args)
        : val_(std::forward<Args>(args)...), has_val_(true) {}

    template <class U, class... Args>
        requires(std::is_constructible_v<T, std::initializer_list<U>&,
                                         Args...>) //
    constexpr explicit expected(std::in_place_t, std::initializer_list<U> il,
                                Args&&... args)
        : val_(il, std::forward<Args>(args)...), has_val_(true) {}

    //
    // In-place construct unexpected value
    //

    template <class... Args>
        requires std::is_constructible_v<E, Args...>
    constexpr explicit expected(unexpect_t, Args&&... args)
        : unex_(std::forward<Args>(args)...), has_val_(false) {}

    template <class U, class... Args>
        requires(std::is_constructible_v<E, std::initializer_list<U>&, Args...>)
    constexpr explicit expected(unexpect_t, std::initializer_list<U> il,
                                Args&&... args)
        : unex_(il, std::forward<Args>(args)...), has_val_(false) {}

    //
    // Destructor
    //

    constexpr ~expected() {
        if (has_val_)
            std::destroy_at(std::addressof(val_));
        else
            std::destroy_at(std::addressof(unex_));
    }

    constexpr ~expected()
        requires(std::conjunction_v<std::is_trivially_destructible<T>,
                                    std::is_trivially_destructible<E>>)
    = default;

    // Copy Assignment Operator

    constexpr expected& operator=(const expected& rhs) {
        if (has_val_ && rhs.has_val_)
            val_ = *rhs;
        else if (has_val_)
            reinit_expected(unex_, val_, rhs.error());
        else if (rhs.has_val_)
            reinit_expected(val_, unex_, *rhs);
        else
            unex_ = rhs.error();

        has_val_ = rhs.has_value();
        return *this;
    }

    constexpr expected& operator=(const expected&)
        requires(!std::conjunction_v<
                    std::is_copy_assignable_v<T>, std::is_copy_constructible<T>,
                    std::is_copy_assignable<E>, std::is_copy_constructible<E>,
                    std::disjunction<std::is_nothrow_move_constructible<E>,
                                     std::is_nothrow_move_constructible<T>>>)
    = delete;

    // Move Assignment Operator

    constexpr expected& operator=(expected&& rhs) noexcept(
        std::conjunction_v<std::is_nothrow_move_assignable<E>,
                           std::is_nothrow_move_constructible<E>,
                           std::is_nothrow_move_assignable<T>,
                           std::is_nothrow_move_constructible<T>>)
        requires(std::conjunction_v<
                 std::is_move_constructible<T>, std::is_move_assignable<T>,
                 std::is_move_constructible<E>, std::is_move_constructible<E>,
                 std::disjunction<std::is_nothrow_move_constructible<T>,
                                  std::is_nothrow_move_constructible<E>>>)
    {
        if (has_val_ && rhs.has_val_)
            val_ = std::move(*rhs);
        else if (has_val_)
            reinit_expected(unex_, val_, std::move(rhs.error()));
        else if (rhs.has_val_)
            reinit_expected(val_, unex_, std::move(*rhs));
        else
            unex_ = std::move(rhs.error());

        has_val_ = rhs.has_val_;
        return *this;
    }

    // Value Assignment Operator

    template <class U = T>
        requires(std::conjunction_v<
                 std::negation<std::is_same<expected, std::remove_cvref_t<U>>>,
                 detail::is_specialization_of<std::remove_cvref_t<U>,
                                              unexpected>,
                 std::is_constructible<T, U>, std::is_assignable<T&, U>,
                 std::disjunction<std::is_nothrow_constructible<T, U>,
                                  std::is_nothrow_move_constructible<T>,
                                  std::is_nothrow_move_constructible<E>>>)
    constexpr expected& operator=(U&& v) {
        if (has_val_)
            val_ = std::forward<U>(v);
        else {
            reinit_expected(val_, unex_, std::forward<U>(v));
            has_val_ = true;
        }
        return *this;
    }

    // Unexpected Copy Assignment Operator

    template <class G, class GF = const G&>
        requires(std::conjunction_v<
                 std::is_constructible<E, GF>, std::is_assignable<E&, GF>,
                 std::disjunction<std::is_nothrow_constructible<E, GF>,
                                  std::is_nothrow_move_constructible<T>,
                                  std::is_nothrow_move_constructible<E>>>)
    constexpr expected& operator=(const unexpected<G>& e) {
        if (has_val_) {
            reinit_expected(unex_, val_, std::forward<GF>(e.error()));
            has_val_ = false;
        } else {
            unex_ = std::forward<GF>(e.error());
        }
        return *this;
    }

    // Unexpected Move Assignment Operator

    template <class G, class GF = G>
        requires(std::conjunction_v<
                 std::is_constructible<E, GF>, std::is_assignable<E&, GF>,
                 std::disjunction<std::is_nothrow_constructible<E, GF>,
                                  std::is_nothrow_move_constructible<T>,
                                  std::is_nothrow_move_constructible<E>>>)
    constexpr expected& operator=(unexpected<G>&& e) {
        if (has_val_) {
            reinit_expected(unex_, val_, std::forward<GF>(e.error()));
            has_val_ = false;
        } else {
            unex_ = std::forward<GF>(e.error());
        }
        return *this;
    }

    template <class... Args>
        requires std::is_nothrow_constructible_v<T, Args...>
    constexpr T& emplace(Args&&... args) noexcept {
        if (has_val_)
            std::destroy_at(std::addressof(val_));
        else {
            std::destroy_at(std::addressof(val_));
            has_val_ = true;
        }
        return *std::construct_at(std::addressof(val_),
                                  std::forward<Args>(args)...);
    }

    template <class U, class... Args>
        requires std::is_nothrow_constructible_v<T,
                                                 std::initializer_list<U>
                                                 &,
                                                 Args...> constexpr T&
        emplace(std::initializer_list<U> li, Args&&... args) noexcept {
        if (has_val_)
            std::destroy_at(std::addressof(val_));
        else {
            std::destroy_at(std::addressof(val_));
            has_val_ = true;
        }
        return *std::construct_at(std::addressof(val_), li,
                                  std::forward<Args>(args)...);
    }

    constexpr void swap(expected& rhs) noexcept(
        std::conjunction_v<std::is_nothrow_move_constructible<T>,
                           std::is_nothrow_swappable<T>,
                           std::is_nothrow_move_constructible<E>,
                           std::is_nothrow_swappable<E>>)
        requires std::conjunction_v<
            std::is_swappable<T>,
            std::is_swappable<E>, std::is_move_constructible<T>,
            std::is_move_constructible<E>,
            std::disjunction<std::is_nothrow_move_constructible<T>,
                             std::is_nothrow_move_constructible<E>>> {
        if (has_val_ && rhs.has_val_) {
            using std::swap;
            swap(val_, rhs.val_);
        } else if (!has_val_ && rhs.has_val_) {
            rhs.swap(*this);
        } else if (!has_val_ && !rhs.has_val_) {
            using std::swap;
            swap(unex_, rhs.unex_);
        } else {
            // rhs.value() is false, this->has_value() is true.

            if constexpr (std::is_nothrow_move_constructible_v<E>) {
                E tmp(std::move(rhs.unex_));
                std::destroy_at(std::addressof(rhs.unex_));
                try {
                    std::construct_at(std::addressof(rhs.val_),
                                      std::move(val_));
                    std::destroy_at(std::addressof(val_));
                    std::construct_at(std::addressof(unex_), std::move(tmp));
                } catch (...) {
                    std::construct_at(std::addressof(rhs.unex_),
                                      std::move(tmp));
                    if constexpr (!std::is_nothrow_move_constructible_v<T>)
                        throw;
                }
            } else {
                T tmp(std::move(val_));
                std::destroy_at(std::addressof(val_));
                try {
                    std::construct_at(std::addressof(unex_),
                                      std::move(rhs.unex_));
                    std::destroy_at(std::addressof(rhs.unex_));
                    std::construct_at(std::addressof(rhs.val_), std::move(tmp));
                } catch (...) {
                    std::construct_at(std::addressof(val_), std::move(tmp));
                    throw;
                }
            }
            has_val_ = false;
            rhs.has_val_ = true;
        }
    }

    friend constexpr void swap(expected& x,
                               expected& y) noexcept(noexcept(x.swap(y))) {
        x.swap(y);
    }

    // Querying

    constexpr explicit operator bool() const noexcept { return has_val_; }
    constexpr bool has_value() const noexcept { return has_val_; }

    // Visitors

    constexpr const T* operator->() const noexcept {
        return std::addressof(val_);
    }
    constexpr T* operator->() noexcept { return std::addressof(val_); }

    constexpr const T& operator*() const& noexcept { return val_; }
    constexpr T& operator*() & noexcept { return val_; }
    constexpr const T&& operator*() const&& noexcept { return std::move(val_); }
    constexpr T&& operator*() && noexcept { return std::move(val_); }

    constexpr const T& value() const& {
        if (has_val_)
            return val_;
        throw bad_expected_access(error());
    }
    constexpr T& value() & {
        if (has_val_)
            return val_;
        throw bad_expected_access(error());
    }

    constexpr const T&& value() const&& {
        if (has_val_)
            return std::move(val_);
        throw bad_expected_access(error());
    }

    constexpr T&& value() && {
        if (has_val_)
            return std::move(val_);
        throw bad_expected_access(error());
    }

    constexpr const E& error() const& { return unex_; }
    constexpr E& error() & { return unex_; }
    constexpr const E&& error() const&& { return std::move(unex_); }
    constexpr E&& error() && { return std::move(unex_); }

    template <class U>
    constexpr T value_or(U&& v) const& {
        static_assert(std::conjunction_v<std::is_copy_constructible<T>,
                                         std::is_convertible<U, T>>);

        return has_val_ ? **this : static_cast<T>(std::forward<U>(v));
    }

    template <class U>
    constexpr T value_or(U&& v) && {
        static_assert(std::conjunction_v<std::is_move_constructible<T>,
                                         std::is_convertible<U, T>>);

        return has_val_ ? std::move(**this)
                        : static_cast<T>(std::forward<U>(v));
    }

#ifdef BST_EXPECTED_MONADIC_INTERFACE
    template <class F>
    constexpr auto and_then(F&& f) &;
    template <class F>
    constexpr auto and_then(F&& f) &&;
    template <class F>
    constexpr auto and_then(F&& f) const&;
    template <class F>
    constexpr auto and_then(F&& f) const&&;

    template <class F>
    constexpr auto or_else(F&& f) &;
    template <class F>
    constexpr auto or_else(F&& f) &&;
    template <class F>
    constexpr auto or_else(F&& f) const&;
    template <class F>
    constexpr auto or_else(F&& f) const&&;

    template <class F>
    constexpr auto transform(F&& f) &;
    template <class F>
    constexpr auto transform(F&& f) &&;
    template <class F>
    constexpr auto transform(F&& f) const&;
    template <class F>
    constexpr auto transform(F&& f) const&&;

    template <class F>
    constexpr auto transform_or(F&& f) &;
    template <class F>
    constexpr auto transform_or(F&& f) &&;
    template <class F>
    constexpr auto transform_or(F&& f) const&;
    template <class F>
    constexpr auto transform_or(F&& f) const&&;
#endif

    // Equality Comparrison

    template <class T2, class E2>
        requires(!std::is_void_v<T2>)
    friend constexpr bool operator==(const expected& x,
                                     const expected<T2, E2>& y) {
        // TODO Mandates

        if (x.has_val_ != y.has_val_)
            return false;
        if (x.has_val_)
            return x.val_ == y.val_;
        else
            return x.unex_ == y.unex_;
    }

    template <class T2>
    friend constexpr bool operator==(const expected& x, const T2& v) {
        // TODO Mandates

        return x.has_val_ && static_cast<bool>(x.val_ == v);
    }

    template <class E2>
    friend constexpr bool operator==(const expected& x,
                                     const unexpected<E2>& e) {
        // TODO Mandates

        return !x.has_val_ && static_cast<bool>(x.unex_ == e.error());
    }

private:
    union {
	struct {} invalid_;
        T val_;
        E unex_;
    };
    bool has_val_;

    template <class T2, class U, class... Args>
    constexpr void reinit_expected(T2& newval, U& oldval, Args&&... args) {
        if constexpr (std::is_nothrow_constructible_v<T2, Args...>) {
            std::destroy_at(std::addressof(oldval));
            std::construct_at(std::addressof(newval),
                              std::forward<Args>(args)...);
        } else if constexpr (std::is_nothrow_move_constructible_v<T2>) {
            T2 tmp(std::forward<Args>(args)...);
            std::destroy_at(std::addressof(oldval));
            std::construct_at(std::addressof(newval), std::move(tmp));
        } else {
            U tmp(std::move(oldval));
            std::destroy_at(std::addressof(oldval));
            try {
                std::construct_at(std::addressof(newval),
                                  std::forward<Args>(args)...);
            } catch (...) {
                std::construct_at(std::addressof(oldval), std::move(tmp));
                throw;
            }
        }
    }
};



//
// class expected<void, E>
//

template <class T, class E>
    requires std::is_void_v<T>
class expected<T, E> {
public:
    using value_type = T;
    using error_type = E;
    using unexpected_type = unexpected<E>;

    template <class U>
    using rebind = expected<U, error_type>;

    constexpr expected() noexcept : has_val_(true) {}

    constexpr expected(const expected& rhs) : has_val_(rhs.has_val_) {
        if (!has_val_)
            std::construct_at(std::addressof(unex_), rhs.error());
    }

    constexpr expected(const expected&)
        requires(!std::is_copy_constructible_v<E>)
    = delete;

    constexpr expected(const expected&)
        requires(std::is_trivially_copy_constructible_v<E>)
    = default;

    constexpr expected(expected&& rhs) noexcept(
        std::is_nothrow_move_constructible_v<E>)
        requires(std::is_move_constructible_v<E>)
    : has_val_(rhs.has_val_) {
        if (!has_val_)
            std::construct_at(std::addressof(unex_), std::move(rhs.error()));
    }

    constexpr expected(expected&& rhs)
        requires(std::is_trivially_move_constructible_v<E>)
    = default;

    template <class U, class G, class GF = const G&>
        requires(std::conjunction_v<
                 std::is_void<U>, std::is_constructible<E, GF>,
                 std::negation<
                     std::is_constructible<unexpected<E>, expected<U, G>&>>,
                 std::negation<
                     std::is_constructible<unexpected<E>, expected<U, G>>>,
                 std::negation<std::is_constructible<unexpected<E>,
                                                     const expected<U, G>&>>,
                 std::negation<std::is_constructible<unexpected<E>,
                                                     const expected<U, G>>>>)
    constexpr explicit(!std::is_convertible_v<GF, E>)
        expected(const expected<U, G>& rhs)
        : has_val_(rhs.has_val_) {
        if (!has_val_)
            std::construct_at(std::addressof(unex_),
                              std::forward<GF>(rhs.error()));
    }

    template <class U, class G, class GF = G>
        requires(std::conjunction_v<
                 std::is_void<U>, std::is_constructible<E, GF>,
                 std::negation<
                     std::is_constructible<unexpected<E>, expected<U, G>&>>,
                 std::negation<
                     std::is_constructible<unexpected<E>, expected<U, G>>>,
                 std::negation<std::is_constructible<unexpected<E>,
                                                     const expected<U, G>&>>,
                 std::negation<std::is_constructible<unexpected<E>,
                                                     const expected<U, G>>>>)
    constexpr explicit(!std::is_convertible_v<GF, E>)
        expected(expected<U, G>&& rhs)
        : has_val_(rhs.has_val_) {
        if (!has_val_)
            std::construct_at(std::addressof(unex_),
                              std::forward<GF>(rhs.error()));
    }

    template <class G, class GF = const G&>
        requires(std::is_constructible_v<E, GF>)
    constexpr explicit(std::is_convertible_v<GF, E>)
        expected(const unexpected<G>& e)
        : has_val_(false) {
        std::construct_at(std::addressof(unex_), std::forward<GF>(e.error()));
    }

    template <class G, class GF = G>
        requires(std::is_constructible_v<E, GF>)
    constexpr explicit(std::is_convertible_v<GF, E>) expected(unexpected<G>&& e)
        : has_val_(false) {
        std::construct_at(std::addressof(unex_), std::forward<GF>(e.error()));
    }

    constexpr explicit expected(std::in_place_t) noexcept : has_val_(true) {}

    template <class... Args>
        requires(std::is_constructible_v<E, Args...>)
    constexpr explicit expected(unexpect_t, Args&&... args)
        : has_val_(false), unex_(std::forward<Args>(args)...) {}

    template <class U, class... Args>
        requires(std::is_constructible_v<E, std::initializer_list<U>&, Args...>)
    constexpr explicit expected(unexpect_t, std::initializer_list<U> il,
                                Args&&... args)
        : has_val_(false), unex_(il, std::forward<Args>(args)...) {}

    constexpr ~expected() {
        if (!has_val_)
            std::destroy_at(std::addressof(unex_));
    }

    constexpr ~expected()
        requires(std::is_trivially_destructible_v<E>)
    = default;

    constexpr expected& operator=(const expected& rhs) {
        if (has_val_ && rhs.has_val_) {
            // No effect
        } else if (has_val_) {
            std::construct_at(std::addressof(unex_), rhs.unex_);
            has_val_ = false;
        } else if (rhs.has_val_) {
            std::destroy_at(std::addressof(unex_));
            has_val_ = false;
        } else {
            unex_ = rhs.unex_;
        }

        return *this;
    }

    constexpr expected& operator=(const expected&)
        requires(!std::conjunction_v<std::is_copy_assignable<E>,
                                     std::is_copy_constructible<E>>)
    = delete;

    constexpr expected& operator=(expected&& rhs) noexcept(
        std::conjunction_v<std::is_nothrow_move_constructible<E>,
                           std::is_nothrow_move_assignable<E>>) {
        if (has_val_ && rhs.has_val_) {
            // No effect
        } else if (has_val_) {
            std::construct_at(std::addressof(unex_), std::move(rhs.unex_));
            has_val_ = false;
        } else if (rhs.has_val_) {
            std::destroy_at(std::addressof(unex_));
            has_val_ = false;
        } else {
            unex_ = std::move(rhs.unex_);
        }

        return *this;
    }

    constexpr expected& operator=(expected&&)
        requires(!std::conjunction_v<std::is_move_assignable<E>,
                                     std::is_move_constructible<E>>)
    = delete;

    template <class G, class GF = const G&>
        requires(std::conjunction_v<std::is_constructible<E, GF>,
                                    std::is_assignable<E&, GF>>)
    constexpr expected& operator=(const unexpected<G>& e) {
        if (has_val_) {
            std::construct_at(std::addressof(unex_),
                              std::forward<GF>(e.error()));
            has_val_ = false;
        } else {
            unex_ = std::forward<GF>(e.error());
        }
        return *this;
    }

    template <class G, class GF = G>
        requires(std::conjunction_v<std::is_constructible<E, GF>,
                                    std::is_assignable<E&, GF>>)
    constexpr expected& operator=(unexpected<G>&& e) {
        if (has_val_) {
            std::construct_at(std::addressof(unex_),
                              std::forward<GF>(e.error()));
            has_val_ = false;
        } else {
            unex_ = std::forward<GF>(e.error());
        }
        return *this;
    }

    constexpr void emplace() noexcept {
        if (!has_val_) {
            std::destroy_at(std::addressof(unex_));
            has_val_ = true;
        }
    }

    constexpr void swap(expected& rhs) noexcept(
        std::conjunction_v<std::is_nothrow_move_constructible<E>,
                           std::is_nothrow_swappable<E>>)
        requires(std::conjunction_v<std::is_swappable<E>,
                                    std::is_move_constructible<E>>)
    {
        if (has_val_ && rhs.has_val_) {
            // No effect.
        } else if (has_val_ && !rhs.has_val_) {
            std::construct_at(std::addressof(unex_), std::move(rhs.unex_));
            std::destroy_at(std::addressof(rhs.unex_));
            has_val_ = false;
            rhs.has_val_ = true;
        } else if (!has_val_ && rhs.has_val_) {
            rhs.swap(*this);
        } else {
            using std::swap;
            swap(unex_, rhs.unex_);
        }
    }

    friend constexpr void swap(expected& x,
                               expected& y) noexcept(noexcept(x.swap(y))) {
        x.swap(y);
    }

    constexpr explicit operator bool() const noexcept { return has_val_; }
    constexpr bool has_value() const noexcept { return has_val_; }
    constexpr void operator*() const noexcept { return; }
    constexpr void value() const& {
        if (!has_val_)
            throw bad_expected_access(error());
    }
    constexpr void value() const&& {
        if (!has_val_)
            throw bad_expected_access(error());
    }

    constexpr const E& error() const& { return unex_; }
    constexpr E& error() & { return unex_; }
    constexpr const E&& error() const&& { return std::move(unex_); }
    constexpr E&& error() && { return std::move(unex_); }

    template <class T2, class E2>
        requires std::is_void_v<T2>
    friend constexpr bool operator==(const expected& x,
                                     const expected<T2, E2>& y) {
        // TODO mandates

        if (x.has_val_ != y.has_val_)
            return false;
        return x.has_val_ || static_cast<bool>(x.error() == y.error());
    }

    template <class E2>
    friend constexpr bool operator==(const expected& x,
                                     const unexpected<E2>& e) {
        // TODO mandates

        return !x.has_val_ && static_cast<bool>(x.error() == e.error());
    }

private:
    union {
        E unex_;
    };
    bool has_val_;
};

} // namespace bst



#endif
