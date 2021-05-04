#ifndef MODULAR_ARITHMETIC_HPP
#define MODULAR_ARITHMETIC_HPP

#include <limits> // numeric_limits
#include <type_traits> // is_same_v, is_unsigned_v, remove_cv_t

template<typename T, T Mod>
class modular_arithmetic final {
public:
    constexpr modular_arithmetic() noexcept = default;
    constexpr modular_arithmetic(T) noexcept;
    constexpr modular_arithmetic(const modular_arithmetic &) noexcept = default;
    constexpr modular_arithmetic(modular_arithmetic &&) noexcept = default;
    constexpr modular_arithmetic &operator=(T) noexcept;
    constexpr modular_arithmetic &operator=(
        const modular_arithmetic &) noexcept = default;
    constexpr modular_arithmetic &operator=(
        modular_arithmetic &&) noexcept = default;
    constexpr ~modular_arithmetic() noexcept = default;

    constexpr modular_arithmetic &operator+=(T) noexcept;
    constexpr modular_arithmetic &operator+=(
        const modular_arithmetic &) noexcept;

    constexpr modular_arithmetic &operator-=(T) noexcept;
    constexpr modular_arithmetic &operator-=(
        const modular_arithmetic &) noexcept;

    constexpr T val() const noexcept;
    constexpr void val(T) noexcept;

private:
    static_assert(is_same_v<T, remove_cv_t<T>>, "");
    static_assert(is_unsigned_v<T>, "");
    static_assert(Mod >= static_cast<T>(2U), "");
    static_assert(numeric_limits<T>::max() / Mod >= Mod, "");

    T val_ = 0U;
};

template<typename T, T Mod>
constexpr modular_arithmetic<T, Mod>::modular_arithmetic(
    const T val
) noexcept : val_(val % Mod) {}

template<typename T, T Mod>
constexpr modular_arithmetic<T, Mod> &modular_arithmetic<T, Mod>::operator=(
    const T val
) noexcept {
    val_ = val % Mod;
}

template<typename T, T Mod>
constexpr modular_arithmetic<T, Mod> &modular_arithmetic<T, Mod>::operator+=(
    const T val
) noexcept {
    assert(val_ < Mod);

    return *this += modular_arithmetic<T, Mod>(val);
}

template<typename T, T Mod>
constexpr modular_arithmetic<T, Mod> &modular_arithmetic<T, Mod>::operator+=(
    const modular_arithmetic<T, Mod> &val
) noexcept {
    assert(val_ < Mod && val.val_ < Mod);

    val_ += val.val_;
    val_ %= Mod;
    return *this;
}

template<typename T, T Mod>
constexpr modular_arithmetic<T, Mod> &modular_arithmetic<T, Mod>::operator-=(
    const T val
) noexcept {
    assert(val_ < Mod);

    return *this -= modular_arithmetic<T, Mod>(val);
}

template<typename T, T Mod>
constexpr modular_arithmetic<T, Mod> &modular_arithmetic<T, Mod>::operator-=(
    const modular_arithmetic<T, Mod> &val
) noexcept {
    assert(val_ < Mod && val.val_ < Mod);

    val_ += Mod - val.val_;
    val_ %= Mod;
    return *this;
}

template<typename T, T Mod>
constexpr T modular_arithmetic<T, Mod>::val() const noexcept {
    return val_;
}

template<typename T, T Mod>
constexpr void modular_arithmetic<T, Mod>::val(const T val) noexcept {
    val_ = val % Mod;
}

template<typename T, T Mod>
constexpr modular_arithmetic<T, Mod> operator+(
    const modular_arithmetic<T, Mod> &val
) noexcept {
    return val;
}

template<typename T, T Mod>
constexpr modular_arithmetic<T, Mod> operator-(
    const modular_arithmetic<T, Mod> &val
) noexcept {
    return modular_arithmetic<T, Mod>(Mod - val.val());
}

template<typename T, T Mod>
constexpr modular_arithmetic<T, Mod> operator+(
    const modular_arithmetic<T, Mod> &lhs,
    const modular_arithmetic<T, Mod> &rhs
) noexcept {
    return modular_arithmetic<T, Mod>(lhs) += rhs;
}

template<typename T, T Mod>
constexpr modular_arithmetic<T, Mod> operator+(
    const modular_arithmetic<T, Mod> &lhs,
    const T rhs
) noexcept {
    return lhs + modular_arithmetic<T, Mod>(rhs);
}

template<typename T, T Mod>
constexpr modular_arithmetic<T, Mod> operator+(
    const T lhs,
    const modular_arithmetic<T, Mod> &rhs
) noexcept {
    return modular_arithmetic<T, Mod>(lhs) + rhs;
}

template<typename T, T Mod>
constexpr modular_arithmetic<T, Mod> operator-(
    const modular_arithmetic<T, Mod> &lhs,
    const modular_arithmetic<T, Mod> &rhs
) noexcept {
    return modular_arithmetic<T, Mod>(lhs) -= rhs;
}

template<typename T, T Mod>
constexpr modular_arithmetic<T, Mod> operator-(
    const modular_arithmetic<T, Mod> &lhs,
    const T rhs
) noexcept {
    return lhs - modular_arithmetic<T, Mod>(rhs);
}

template<typename T, T Mod>
constexpr modular_arithmetic<T, Mod> operator-(
    const T lhs,
    const modular_arithmetic<T, Mod> &rhs
) noexcept {
    return modular_arithmetic<T, Mod>(lhs) - rhs;
}

#endif
