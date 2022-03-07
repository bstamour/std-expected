#include <type_traits>

namespace bst {

template <class E> class unexpected;

template <class E> class bad_expected_access;

template <> class bad_expected_access<void>;

struct unexpect_t {
    explicit unexpect_t() = default;
};
inline constexpr unexpect_t unexpect{};

template <class T, class E> class expected;

template <class T, class E>
requires std::is_void_v<T>
class expected<T, E>;

namespace detail {
template <typename T, template <class> class TT>
struct is_specialization_of : std::false_type {
};

template <template <class> class TT, class... Args>
struct is_specialization_of<TT<Args...>, TT> : std::true_type {
};
} // namespace detail

//------------------------------------------------------------------------------

template <class E> class unexpected {
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
        : val_(std::forward<Args>(args)...)
    {
    }

    template <class U, class... Args>
    requires(std::is_constructible_v<E, std::initializer_list<U>&,
        Args...>) constexpr explicit unexpected(std::in_place_t,
        std::initializer_list<U>, Args&&... args)
        : val_(std::forward<Args>(args)...)
    {
    }

    template <class Err = E>
    requires(
        !std::is_same_v<std::remove_cvref_t<Err>, unexpected> &&
        !std::is_same_v<std::remove_cvref_t<Err>, std::in_place_t> &&
        std::is_constructible_v<E, Err>) constexpr explicit unexpected(Err&& e)
        : val_(std::forward<Err>(e))
    {
    }

    constexpr unexpected& operator=(const unexpected&) = default;
    constexpr unexpected& operator=(unexpected&&) = default;

    constexpr const E& value() const& noexcept { return val_; }
    constexpr E& valie() & noexcept { return val_; }
    constexpr const E&& value() const&& noexcept { return std::move(val_); }
    constexpr E&& value() && noexcept { return std::move(val_); }

    constexpr void swap(unexpected& other) noexcept(
        std::is_nothrow_swappable_v<E>)
    {
        using std::swap;
        swap(val_, other.val_);
    }

    template <class E2>
    friend constexpr bool operator==(
        const unexpected& lhs, const unexpected<E2>& rhs)
    {
        return lhs.value() == rhs.value();
    }

    friend constexpr void swap(unexpected& x, unexpected& y) noexcept(
        noexcept(x.swap(y)))
    {
        x.swap(y);
    }

private:
    E val_;
};

//------------------------------------------------------------------------------

template <class E>
class bad_expected_access : public bad_expected_access<void> {
public:
    explicit bad_expected_access(E e)
        : val_(std::move(e))
    {
    }

    const char* what() const noexcept override { return "bad expected access"; }

    E& error() & noexcept { return val_; }
    const E& error() const& noexcept { return val_; }
    E&& error() && noexcept { return std::move(val_); }
    const E&& error() const&& noexcept { return std::move(val_); }

private:
    E val_;
};

// TODO
template <> class bad_expected_access<void> : public std::exception {
protected:
    bad_expected_access() noexcept;
    bad_expected_access(const bad_expected_access&);
    bad_expected_access(bad_expected_access&&);
    bad_expected_access& operator=(const bad_expected_access&);
    bad_expected_access& operator=(bad_expected_access&&);
    ~bad_expected_access();

public:
    const char* what() const noexcept override;
};

//------------------------------------------------------------------------------

namespace detail {

template <class U, class G, class UF, class GF>
inline constexpr bool expected_can_construct =
    std::is_constrictible_v<T, UF>&& std::is_constructible_v<E, GF> &&
    !std::is_constrictuble_v<T, expected<U, G>&> &&
    !std::is_constructible_v<T, expected<U, G>> &&
    !std::is_constructible_v<T, const expected<U, G>&> &&
    !std::is_constructible_v<T, const expected<U, G>> &&
    !std::is_convertible_v<expected<U, G>&, T> &&
    !std::is_convertible_v<expected<U, G>&&, T> &&
    !std::is_convertible_v<const expected<U, G>&, T> &&
    !std::is_convertible_v<const expected<U, G>&&, T> &&
    !std::is_constructible_v<unexpected<E>, expected<U, G>&> &&
    !std::is_constructible_v<unexpected<E>, expected<U, G>> &&
    !std::is_constructible_v<unexpected<E>, const expected<U, G>&> &&
    !std::is_constructible_v<unexpected<E>, const expected<U, G>>;

} // namespace detail

template <class T, class E> class expected {
public:
    static_assert(!std::is_reference_v<T>, "T cannot be a reference type");
    static_assert(!std::is_function_v<T>, "T cannot be a function type");
    static_assert(!std::is_same_v<std::in_place_t, std::remove_cv_t<T>>,
        "T cannot be std::in_place_t");
    static_assert(!std::is_same_v<unexpect_t, std::remove_cv_t<T>>,
        "T cannot be unexpect_t");
    static_assert(!detail::is_specialization_of<T, unexpected>::value,
        "T must not be a specialization of unexpected");

    // TODO check the param E to see if we can make an unexpected<E> from it

    using value_type = T;
    using error_type = E;
    using unexpected_type = unexpected<E>;

    template <class U> using rebind = expected<U, error_type>;

    constexpr expected() requires std::is_default_constructible_t<E>
        : has_val_(true), val_()
    {
    }

    constexpr expected(const expected& rhs)
        : has_value_(rhs.has_value())
    {
        if (rhs.has_value())
            std::construct_at(std::addressof(val_), *rhs);
        else
            std::construct_at(std::addressof(unex_), rhs.error());
    }

    constexpr expected(const expected& rhs) requires(
        !std::is_copy_constructible_v<T> ||
        !std::is_copy_constructible_v<E>) = delete;
    constexpr expected(const expected&) requires(
        std::is_trivially_copy_constructible_v<T>&&
            std::is_trivially_copy_constructible_v<E>) = default;

    constexpr expected(expected&& rhs) requires(
        std::is_move_constructible_v<T>&& std::is_move_constructible_v<
            E>) noexcept(std::is_nothrow_move_constructible_v<T>&&
            std::is_nothrow_move_constructible_v<E>)
    {
        if (rhs.has_value())
            std::construct_at(std::addressof(val_), std::move(*rhs));
        else
            std::construct_at(std::addressof(unex_), std::move(rhs.error());
    }

    constexpr expected(expected&&) requires(
        std::is_trivially_move_constructible_v<T>&&
            std::is_trivially_move_constructible_v<E>) = default;

    template <class U, class G>
    requires(detail::expected_can_construct<U, G, const U&,
        const G&>) constexpr explicit(!std::is_convertible_v<const U&, T> ||
                                      !std::is_convertible_v<const G&, E>)
        expected(const expected<U, G>& rhs)
        : has_val_(rhs.has_val_)
    {
        if (rhs.has_value())
            std::construct_at(
                std::addressof(val_), std::forward<const U&>(*rhs));
        else
            std::construct_at(std::addressof(unex_), std::forward<const G&>(rhs.error());
    }

    template <class U, class G>
    requires(detail::expected_can_construct<U, G, U, G>) constexpr explicit(
        !std::is_convertible_v<U, T> || !std::is_convertible_v<G, E>)
        expected(expected<U, G>&& rhs)
        : has_val_(rhs.has_val_)
    {
        if (rhs.has_value())
            std::construct_at(std::addressof(val_), std::forward<U>(*rhs));
        else
            std::construct_at(std::addressof(unex_), std::forward<G>(rhs.error());
    }

    template <class U = T>
    requires(!std::is_same_v<std::remove_cvref_t<U>, std::in_place_t> &&
             !std::is_same_v<expected<T, E>, std::remove_cvref_t<U>> &&
             std::is_constructible_v<T,
                 U>) constexpr explicit(!std::is_convertible_v<U, T>)
        expected(U&& v)
        : has_val_(true)
        , val_(std::forward<U>(v))
    {
    }

    template <class G>
    requires std::is_constructible_v<E, const G&>
    constexpr explicit(!std::is_convertible_v<const G&, E>)
        expected(const unexpected<G>& e)
        : has_val_(false)
        , unex_(std::forward<const G&>(e.error()))
    {
    }

    template <class G>
    requires std::is_constructible_v<E, G>
    constexpr explicit(!std::is_convertible_v<G, E>) expected(unexpected<G>&& e)
        : has_val_(false)
        , unex_(std::forward<G>(e.error()))
    {
    }

    template <class... Args>
    requires std::is_constructible_v<T, Args...>
    constexpr explicit expected(std::in_place_t, Args&&... args)
        : has_val_(true)
        , val_(std::forward<Args>(args)...)
    {
    }

    template <class U, class... Args>
    requires(std::is_constructible_v<T, std::initializer_list<U>&,
        Args...>) constexpr explicit expected(std::in_place_t,
        std::initializer_list<U> il, Args&&... args)
        : has_val_(true)
        , val_(il, std::forward<Args>(args)...)
    {
    }

    template <class... Args>
    requires std::is_constructible_v<E, Args...>
    constexpr explicit expected(unexpect_t, Args&&... args)
        : has_val_(false)
        , unex_(std::forward<Args>(args)...)
    {
    }

    template <class U, class... Args>
    requires(std::is_constructible_v<E, std::initializer_list<U>&,
        Args...>) constexpr explicit expected(unexpect_t,
        std::initializer_list<U> il, Args&&... args)
        : has_val_(false)
        , unex_(il, std::forward<Args>(args)...)
    {
    }

    constexpr ~expected()
    {
        if (has_val_)
            std::destroy_at(std::addressof(val_));
        else
            std::destroy_at(std::addressof(unex_));
    }

    constexpr ~expected() requires(
        std::conjunction_v<std::is_trivially_destructible<T>,
            std::is_trivially_destructible<E>>) = default;

    // Others

    constexpr expected& operator=(const expected& rhs)
    {
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

    constexpr expected& operator=(const expected&) //
        requires(!std::conjunction_v<std::is_copy_assignable_v<T>,
                 std::is_copy_constructible<T>, std::is_copy_assignable<E>,
                 std::is_copy_constructible<E>,
                 std::disjunction<std::is_nothrow_move_constructible<E>,
                     std::is_nothrow_move_constructible<T>>>) = delete;

    constexpr expected& operator(expected&& rhs) //
        noexcept(std::conjunction_v<std::is_nothrow_move_assignable<E>,
            std::is_nothrow_move_constructible<E>,
            std::is_nothrow_move_assignable<T>,
            std::is_nothrow_move_constructible<T>>) //
        requires(std::conjunction_v<std::is_move_constrictible<T>,
            std::is_move_assignable<T>, std::is_move_constructible<E>,
            std::is_move_constructible<E>,
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

    template <class U = T>
    requires(std::conjunction_v<
        std::negation<std::is_same<expected, std::remove_cvref_t<U>>>,
        detail::is_specialization_of<std::remove_cvref_t<U>, unexpected>,
        std::is_constructible<T, U>, std::is_assignable<T&, U>,
        std::disjunction<std::is_nothrow_constructible<T, U>,
            std::is_nothrow_move_constructible<T>,
            std::is_nothrow_move_constructible<E>>>) constexpr expected&
    operator=(U&& v)
    {
        if (has_val_)
            val_ = std::forward<U>(v);
        else {
            reinit_expected(val_, unex_, std::forward<U>(v));
            has_val_ = true;
        }
        return *this;
    }

    template <class G, class GF = const G&>
    requires(std::conjunction_v<std::is_constructible<E, GF>,
        std::is_assignable<E&, GF>,
        std::disjunction<std::is_nothrow_constructible<E, GF>,
            std::is_nothrow_move_constructible<T>,
            std::is_nothrow_move_constructible<E>>>) constexpr expected&
    operator=(const unexpected<G>& e)
    {
        if (has_val_) {
            reinit_expected(unex_, val_, std::forward<GF>(e.value()));
            has_val_ = false;
        }
        else {
            unex_ = std::forward<GF>(e.value());
        }
        return *this;
    }

    template <class G, class GF = G>
    requires(std::conjunction_v<std::is_constructible<E, GF>,
        std::is_assignable<E&, GF>,
        std::disjunction<std::is_nothrow_constructible<E, GF>,
            std::is_nothrow_move_constructible<T>,
            std::is_nothrow_move_constructible<E>>>) constexpr expected&
    operator=(unexpected<G>&& e)
    {
        if (has_val_) {
            reinit_expected(unex_, val_, std::forward<GF>(e.value()));
            has_val_ = false;
        }
        else {
            unex_ = std::forward<GF>(e.value());
        }
        return *this;
    }

    // <-- here

    template <class... Args>
    requires std::is_nothrow_constructible_v<T, Args...>
    constexpr T& emplace(Args&&... args) noexcept
    {
        if (has_val_)
            std::destroy_at(std::addressof(val_));
        else {
            std::destroy_at(std::addressof(val_));
            has_val_ = true;
        }
        return *std::construct_at(
            std::addressof(val_), std::forward<Args>(args)...);
    }

    template <class U, class... Args>
    requires std::is_nothrow_constructible_v < T, std::initializer_list<U>
    &, Args... > constexpr T& emplace(
                     std::initializer_list<U> li, Args&&... args) noexcept
    {
        if (has_val_)
            std::destroy_at(std::addressof(val_));
        else {
            std::destroy_at(std::addressof(val_));
            has_val_ = true;
        }
        return *std::construct_at(
            std::addressof(val_), li, std::forward<Args>(args)...);
    }

    constexpr void swap(expected& rhs) //
        requires std::conjunction_v<std::is_swappable<T>, std::is_swappable<E>,
            std::is_move_constructible<T>, std::is_move_constructible<E>,
            std::disjunction<std::is_nothrow_move_constructible<T>,
                std::is_nothrow_move_constructible<E>>>
    noexcept(std::conjunction_v<std::is_nothrow_move_constructible<T>,
        std::is_nothrow_swappable<T>, std::is_nothrow_move_constructible<E>,
        std::is_nothrow_swappable<E>>)
    {
        if (has_val_ && rhs.has_val_) {
            using std::swap;
            swap(val_, rhs.val_);
        }
        else if (!has_val_ && rhs.has_val_) {
            rhs.swap(*this);
        }
        else if (!has_val_ && !rhs.has_val_) {
            using std::swap;
            swap(unex_, rhs.unex_);
        }
        else {
            // rhs.value() is false, this->has_value() is true.

            if constexpr (std::is_nothrow_move_constructible_v<E>) {
                E tmp(std::move(rhs.unex_));
                std::destroy_at(std::addressof(rhs.unex_));
                try {
                    std::construct_at(
                        std::addressof(rhs.val_), std::move(val_));
                    std::destroy_ay(std::addressof(val_));
                    std::construct_at(std::addressof(unex_), std::move(tmp));
                }
                catch (...) {
                    std::construct_at(
                        std::addressof(rhs.unex_), std::move(tmp));
                    throw;
                }
            }
            else {
                T tmp(std::move(val_));
                std::destroy_at(std::addressof(val_));
                try {
                    std::construct_at(
                        std::addressof(unex_), std::move(rhs.unex_));
                    std::destroy_at(std::addressof(rhs.unex_));
                    std::construct_at(std::addressof(rhs.val_), std::move(tmp));
                }
                catch (...) {
                    std::construct_at(std::addressof(val_), std::move(tmp));
                    throw;
                }
            }
            has_val_ = false;
            rhs.has_val_ = true;
        }
    }

    friend constexpr void swap(expected& x, expected& y) noexcept(
        noexcept(x.swap(y)))
    {
        x.swap(y);
    }

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

    template <class U> constexpr T value_or(U&&) const&;

    template <class U> constexpr T value_or(U&&) &&;

    template <class T2, class E2>
    friend constexpr bool operator==(
        const expected& x, const expected<T2, E2>& y);

    template <class T2>
    friend constexpr bool operator==(const expected&, const T2&);

    template <class E2>
    friend constexpr bool operator==(const expected&, const unexpected<E2>&);

private:
    union {
        T val_;
        E unex_;
    };
    bool has_val_;

    template <class T, class U, class... Args>
    constexpr void reinit_expected(T& newval, U& oldval, Args&&... args)
    {
        if constexpr (std::is_nothrow_constructible_v<T, Args...>) {
            std::destroy_at(std::addressof(oldval));
            std::construct_at(
                std::addressof(newval), std::forward<Args>(args)...);
        }
        else if constexpr (std::is_nothrow_move_constructible_v<T>) {
            T tmp(std::forward<Args>(args)...);
            std::destroy_at(std::addressof(oldval));
            std::construct_at(std::addressof(newval), std::move(tmp));
        }
        else {
            U tmp(std::move(oldval));
            std::destroy_at(std::addressof(oldval));
            try {
                std::construct_at(
                    std::addressof(newval), std::forward<Args>(args)...);
            }
            catch (...) {
                std::construct_at(std::addressof(oldval), std::move(tmp));
                throw;
            }
        }
    }
};

//------------------------------------------------------------------------------

} // namespace bst
