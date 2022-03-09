# std-expected
An implementation of std::expected from the upcomming C++23 ISO standard

# Needs testing
Everything.

# Overview

```c++
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

    constexpr const E& value() const& noexcept;
    constexpr E& valie() & noexcept;
    constexpr const E&& value() const&& noexcept;
    constexpr E&& value() && noexcept;

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
```
