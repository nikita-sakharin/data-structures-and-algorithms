#ifndef _BIG_INTEGER_H_
#define _BIG_INTEGER_H_

#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
#include <vector>
#include <utility>

class BigInteger
{
private:
    typedef unsigned long long Integer;

    static constexpr const Integer RADIX = 1000000000ull;/*10ull*/
    static constexpr const Integer HALF_OF_RADIX = 500000000ull;/*5ull*/
    static constexpr const unsigned TEN = 10u;
    static constexpr const unsigned DIGITS = 9u;/*1u*/

    std::vector<Integer> repres;

    Integer get(std::vector<BigInteger::Integer>::size_type) const;

    BigInteger &operator+=(const Integer &);
    BigInteger &operator-=(const Integer &);
    BigInteger &operator*=(const Integer &);
    BigInteger &operator/=(const Integer &);

    BigInteger operator+(const Integer &) const;
    BigInteger operator-(const Integer &) const;
    BigInteger operator*(const Integer &) const;
    BigInteger operator/(const Integer &) const;

public:
    BigInteger();
    BigInteger(const BigInteger &);
    BigInteger(BigInteger &&) noexcept;
    template <typename T>
    BigInteger(const T &);
    template <typename T>
    BigInteger(T &&);

    ~BigInteger();

    BigInteger &operator=(const BigInteger &);
    BigInteger &operator=(BigInteger &&);

    bool operator<(const BigInteger &) const noexcept;
    bool operator<=(const BigInteger &) const noexcept;
    bool operator==(const BigInteger &) const noexcept;
    bool operator>=(const BigInteger &) const noexcept;
    bool operator>(const BigInteger &) const noexcept;

    BigInteger &operator++();
    BigInteger &operator--();

    BigInteger &operator+=(const BigInteger &);
    BigInteger &operator-=(const BigInteger &);
    BigInteger &operator*=(const BigInteger &);
    BigInteger &operator/=(const BigInteger &);
    friend BigInteger &power_eq(BigInteger &, const BigInteger &);

    BigInteger operator+(const BigInteger &) const;
    BigInteger operator-(const BigInteger &) const;
    BigInteger operator*(const BigInteger &) const;
    BigInteger operator/(const BigInteger &) const;
    friend BigInteger power(const BigInteger &, const BigInteger &);

    friend std::ostream &operator<<(std::ostream &, const BigInteger &);
    friend std::istream &operator>>(std::istream &, BigInteger &);
};

constexpr const BigInteger::Integer BigInteger::RADIX;

BigInteger::Integer BigInteger::get(std::vector<BigInteger::Integer>::size_type idx) const
{
    if (idx >= repres.size())
    {
        return 0u;
    }

    return repres[idx];
}

BigInteger &BigInteger::operator+=(const Integer &obj)
{
    if (!obj)
    {
        return *this;
    }
    if (repres.empty())
    {
        repres.push_back(obj);
    }
    else
    {
        repres.front() += obj;
    }

    register Integer carry(0u);
    for (auto it(repres.begin()), end(repres.end()); it != end; ++it)
    {
        *it += carry;
        if (*it >= RADIX)
        {
            carry = 1u;
            *it %= RADIX;
        }
        else
        {
            carry = 0u;
            break;
        }
    }
    if (carry)
    {
        repres.push_back(1u);
    }

    return *this;
}

BigInteger &BigInteger::operator-=(const Integer &obj)
{
    if (!obj)
    {
        return *this;
    }
    else if (repres.empty() || (repres.size() == 1u && repres.front() < obj))
    {
        throw std::range_error("Unsigned subtraction yielding a negative value");
    }

    repres.front() -= obj;
    register Integer carry(0u);
    for (auto it(repres.begin()), end(repres.end()); it != end; ++it)
    {
        *it -= carry;
        if (*it >= RADIX)
        {
            carry = 1u;
            *it += RADIX;
        }
        else
        {
            carry = 0;
            break;
        }
    }
    if (!repres.back())
    {
        repres.pop_back();
    }

    return *this;
}

BigInteger &BigInteger::operator*=(const Integer &obj)
{
    if (repres.empty())
    {
        return *this;
    }
    else if (!obj)
    {
        this->repres.clear();
        return *this;
    }

    Integer carry(0u);
    for (auto i_it(repres.begin()), i_end(repres.end()); i_it != i_end; ++i_it)
    {
        *i_it *= obj;
        *i_it += carry;
        carry = *i_it / RADIX;
        *i_it %= RADIX;
    }
    if (carry)
    {
        repres.push_back(carry);
    }

    return *this;
}

BigInteger &BigInteger::operator/=(const Integer &obj)
{
    if (!obj)
    {
        throw std::overflow_error("Division by zero");
    }
    if (repres.empty() || obj == 1u)
    {
        return *this;
    }
    else if (repres.size() == 1u)
    {
        repres.front() /= obj;
        return *this;
    }

    register Integer remainder(0u);
    for (auto i_it(repres.rbegin()), i_end(repres.rend()); i_it != i_end; ++i_it)
    {
        remainder *= RADIX;
        remainder += *i_it;
        *i_it = remainder / obj;
        remainder %= obj;
    }
    if (!repres.back())
    {
        repres.pop_back();
    }

    return *this;
}

BigInteger BigInteger::operator+(const Integer &obj) const
{
    BigInteger res(*this);
    res += obj;

    return res;
}

BigInteger BigInteger::operator-(const Integer &obj) const
{
    BigInteger res(*this);
    res -= obj;

    return res;
}

BigInteger BigInteger::operator*(const Integer &obj) const
{
    BigInteger res(*this);
    res *= obj;

    return res;
}

BigInteger BigInteger::operator/(const Integer &obj) const
{
    BigInteger res(*this);
    res /= obj;

    return res;
}

BigInteger::BigInteger() : repres()
{
}

BigInteger::BigInteger(const BigInteger &obj) : repres(obj.repres)
{
}

BigInteger::BigInteger(BigInteger &&obj) noexcept : repres(std::move(obj.repres))
{
}

template <typename T>
BigInteger::BigInteger(const T &obj) : repres()
{
    if (obj < 0)
    {
        throw std::range_error("Negative value");
    }

    T temp(obj);
    while (temp)
    {
        repres.push_back(temp % RADIX);
        temp /= RADIX;
    }
}

template <typename T>
BigInteger::BigInteger(T &&obj) : repres()
{
    T temp(obj);
    while (temp)
    {
        repres.push_back(temp % RADIX);
        temp /= RADIX;
    }
}

BigInteger::~BigInteger()
{
}

BigInteger &BigInteger::operator=(const BigInteger &obj)
{
    if (this != &obj)
    {
        repres = obj.repres;
    }

    return *this;
}

BigInteger &BigInteger::operator=(BigInteger &&obj)
{
    repres = std::move(obj.repres);

    return *this;
}

bool BigInteger::operator<(const BigInteger &obj) const noexcept
{
    if (repres.size() > obj.repres.size())
    {
        return false;
    }
    else if (repres.size() < obj.repres.size())
    {
        return true;
    }
    else
    {
        auto mis = std::mismatch(repres.crbegin(), repres.crend(), obj.repres.crbegin());
        return (mis.first != repres.crend()) && (*mis.first < *mis.second);
    }
}

bool BigInteger::operator<=(const BigInteger &obj) const noexcept
{
    if (repres.size() > obj.repres.size())
    {
        return false;
    }
    else if (repres.size() < obj.repres.size())
    {
        return true;
    }
    else
    {
        auto mis = std::mismatch(repres.crbegin(), repres.crend(), obj.repres.crbegin());
        return (mis.first == repres.crend()) || (*mis.first < *mis.second);
    }
}

bool BigInteger::operator==(const BigInteger &obj) const noexcept
{
    return this->repres == obj.repres;
}

bool BigInteger::operator>=(const BigInteger &obj) const noexcept
{
    if (repres.size() > obj.repres.size())
    {
        return true;
    }
    else if (repres.size() < obj.repres.size())
    {
        return false;
    }
    else
    {
        auto mis = std::mismatch(repres.crbegin(), repres.crend(), obj.repres.crbegin());
        return (mis.first == repres.crend()) || (*mis.first > *mis.second);
    }
}

bool BigInteger::operator>(const BigInteger &obj) const noexcept
{
    if (repres.size() > obj.repres.size())
    {
        return true;
    }
    else if (repres.size() < obj.repres.size())
    {
        return false;
    }
    else
    {
        auto mis = std::mismatch(repres.crbegin(), repres.crend(), obj.repres.crbegin());
        return (mis.first != repres.crend()) && (*mis.first > *mis.second);
    }
}

BigInteger &BigInteger::operator++()
{
    *this += Integer(1u);

    return *this;
}

BigInteger &BigInteger::operator--()
{
    *this -= Integer(1u);

    return *this;
}

BigInteger &BigInteger::operator+=(const BigInteger &obj)
{
    if (obj.repres.empty())
    {
        return *this;
    }
    else if (obj.repres.size() == 1u)
    {
        *this += obj.repres.front();
        return *this;
    }
    auto it(repres.begin()), end(repres.end());
    auto op_it(obj.repres.cbegin()), op_end(obj.repres.cend());
    register Integer carry(0u);
    for (; it != end; ++it)
    {
        if (op_it != op_end)
        {
            *it += *op_it + carry;
            ++op_it;
        }
        else if (carry)
        {
            *it += carry;
        }
        else
        {
            break;
        }
        if (*it >= RADIX)
        {
            carry = 1u;
            *it %= RADIX;
        }
        else
        {
            carry = 0u;
        }
    }
    for (; op_it != op_end; ++op_it)
    {
        repres.push_back(*op_it + carry);
        if (repres.back() >= RADIX)
        {
            carry = 1u;
            repres.back() %= RADIX;
        }
        else
        {
            carry = 0u;
        }
    }
    if (carry)
    {
        repres.push_back(1u);
    }

    return *this;
}

BigInteger &BigInteger::operator-=(const BigInteger &obj)
{
    if (obj.repres.empty())
    {
        return *this;
    }
    else if (obj.repres.size() == 1u)
    {
        *this -= obj.repres.front();
        return *this;
    }
    else if (repres.size() < obj.repres.size())
    {
        throw std::range_error("Unsigned subtraction yielding a negative value");
    }

    auto it(repres.begin()), end(repres.end());
    auto op_it(obj.repres.cbegin()), op_end(obj.repres.cend());
    auto all_zero(end);
    register Integer carry(0u);
    for (; it != end; ++it)
    {
        if (op_it != op_end)
        {
            *it -= *op_it + carry;
            ++op_it;
        }
        else if (carry)
        {
            *it -= carry;
        }
        else
        {
            all_zero = end;
            break;
        }
        if (*it >= RADIX)
        {
            carry = 1u;
            *it += RADIX;
        }
        else
        {
            carry = 0;
        }
        if (*it)
        {
            all_zero = end;
        }
        else if (all_zero == end)
        {
            all_zero = it;
        }
    }
    if (carry)
    {
        throw std::range_error("Unsigned subtraction yielding a negative value");
    }
    if (all_zero != end)
    {
        repres.resize(all_zero - repres.begin());
    }

    return *this;
}

BigInteger &BigInteger::operator*=(const BigInteger &obj)
{
    *this = *this * obj;
    return *this;
}

BigInteger &BigInteger::operator/=(const BigInteger &obj)
{
    if (obj.repres.empty())
    {
        throw std::overflow_error("Division by zero");
    }
    if (repres.size() < obj.repres.size() || (repres.size() == obj.repres.size() && repres.back() < obj.repres.back()))
    {
        this->repres.clear();
        return *this;
    }
    if (obj.repres.size() == 1u)
    {
        *this /= obj.repres.front();
        return *this;
    }

    BigInteger v(obj);
    if (v.repres.back() < HALF_OF_RADIX)
    {
        Integer norm(RADIX / (v.repres.back() + 1u));
        *this *= norm;
        v *= norm;
    }

    BigInteger q, remainder, sub;
    q.repres.resize(repres.size() - obj.repres.size() + 1u);
    remainder.repres.resize(v.repres.size() - 1u);
    std::copy(repres.crbegin(), repres.crbegin() + remainder.repres.size(), remainder.repres.rbegin());

    register const Integer v_n__1(v.repres.back()), v_n__2(*(v.repres.crbegin() + 1u));
    register const std::size_t n(v.repres.size());
    auto i_it(repres.crbegin() + remainder.repres.size());
    auto zero_it(q.repres.crbegin());
    bool is_find_non_zero = false;
    const BigInteger radix(RADIX);
    for (auto j_it(q.repres.rbegin()), j_end(q.repres.rend()); j_it != j_end; ++i_it, ++j_it)
    {
        remainder *= radix;
        remainder += *i_it;

        register const Integer u_n(remainder.get(n)), u_n__1(remainder.get(n - 1u)), u_n__2(remainder.get(n - 2u));
        Integer q_approxim((u_n * RADIX + u_n__1) / v_n__1), r_approxim((u_n * RADIX + u_n__1) % v_n__1);

        while ((r_approxim < RADIX) && ((q_approxim == RADIX) || (q_approxim * v_n__2 > RADIX * r_approxim + u_n__2)))
        {
            --q_approxim;
            r_approxim += v_n__1;
        }
        sub = v * q_approxim;
        if (remainder < v * q_approxim)
        {
            --q_approxim;
            sub -= v;
        }
        *j_it = q_approxim;
        remainder -= sub;

        if (*j_it)
        {
            is_find_non_zero = true;
        }
        else if (!is_find_non_zero)
        {
            ++zero_it;
        }
    }
    if (zero_it != q.repres.crbegin())
    {
        q.repres.resize(q.repres.crend() - zero_it);
    }

    *this = std::move(q);

    return *this;
}

BigInteger &power_eq(BigInteger &base, const BigInteger &exp)
{
    const BigInteger one(1u);
    if (base.repres.empty())
    {
        if (exp.repres.empty())
        {
            throw std::range_error("Division by zero");
        }
        else
        {
            return base;
        }
    }
    else if (base == one)
    {
        return base;
    }
    if (exp.repres.empty())
    {
        base = one;
        return base;
    }
    else if (exp == one)
    {
        return base;
    }

    BigInteger increm(1u), residue(exp);
    do
    {
        if (residue.repres.back() & 1u)
        {
            increm *= base;
            --residue;
        }
        else
        {
            base *= base;
            residue /= 2u;
        }
    } while (residue > one);
    base *= increm;

    return base;
}

BigInteger BigInteger::operator+(const BigInteger &obj) const
{
    BigInteger res(*this);
    res += obj;

    return res;
}

BigInteger BigInteger::operator-(const BigInteger &obj) const
{
    BigInteger res(*this);
    res -= obj;

    return res;
}

BigInteger BigInteger::operator*(const BigInteger &obj) const
{
    if (repres.empty() || obj.repres.empty())
    {
        return BigInteger();
    }
    else if (obj.repres.size() == 1u)
    {
        return *this * obj.repres.front();
    }

    BigInteger res;
    res.repres.resize(repres.size() + obj.repres.size());

    auto j_it(obj.repres.cbegin()), j_end(obj.repres.cend()), i_it(repres.cbegin()), i_begin(repres.cbegin()), i_end(repres.cend());
    Integer carry(0u);
    for (auto res_it(res.repres.begin()), local_res_it(res.repres.begin()); j_it != j_end; ++j_it, ++res_it)
    {
        if (!*j_it)
        {
            continue;
        }
        for (i_it = i_begin, local_res_it = res_it; i_it != i_end; ++i_it, ++local_res_it)
        {
            *local_res_it += *j_it * *i_it + carry;
            carry = *local_res_it / RADIX;
            *local_res_it %= RADIX;
        }
        if (carry)
        {
            *local_res_it += carry;
            carry = 0u;
        }
    }
    if (!res.repres.back())
    {
        res.repres.pop_back();
    }

    return res;
}

BigInteger BigInteger::operator/(const BigInteger &obj) const
{
    BigInteger res(*this);
    res /= obj;

    return res;
}

BigInteger power(const BigInteger &base, const BigInteger &exp)
{
    BigInteger res(base);
    power_eq(res, exp);

    return res;
}

std::ostream &operator<<(std::ostream &stream, const BigInteger &obj)
{
    if (obj.repres.empty())
    {
        stream << "0";

        return stream;
    }

    stream << obj.repres.back();
    for (auto it(obj.repres.crbegin() + 1u), end(obj.repres.crend()); it != end; ++it)
    {
        stream << std::setfill('0') << std::setw(BigInteger::DIGITS) << *it;
    }

    return stream;
}

std::istream &operator>>(std::istream &stream, BigInteger &obj)
{
    obj.repres.resize(1u);
    obj.repres.back() = 0u;

    std::string buffer;
    stream >> buffer;

    register BigInteger::Integer digit = 1u;
    for (auto it(buffer.crbegin()), end(buffer.crend()); it != end; ++it, digit *= BigInteger::TEN)
    {
        if (digit >= obj.RADIX)
        {
            obj.repres.push_back(0u);
            digit = 1u;
        }
        obj.repres.back() += (*it - '0') * digit;
    }

    auto it(obj.repres.crbegin()), end(obj.repres.crend());
    while (it != end && !*it)
    {
        ++it;
    }
    obj.repres.resize(end - it);

    return stream;
}

#endif
