#include <cstddef>

#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

typedef signed char schar;
typedef unsigned char uchar;
typedef short shrt;
typedef unsigned short ushrt;
typedef unsigned uint;
typedef unsigned long ulong;
typedef long long llong;
typedef unsigned long long ullong;
typedef float flt;
typedef double dbl;
typedef long double ldbl;

static constexpr size_t TWO = 2U;

// fastest_way - O(n)

ullong fastest_way(const array<vector<ullong>, TWO> &,
    const array<vector<ullong>, TWO> &, const array<ullong, TWO> &,
    const array<ullong, TWO> &) noexcept;

int main()
{
    size_t n;
    cin >> n;
    array<vector<ullong>, TWO> a, t;
    array<ullong, TWO> e, x;

    for (vector<ullong> &a_i : a)
    {
        a_i.resize(n);
        for (ullong &a_i_j : a_i)
            cin >> a_i_j;
    }
    for (vector<ullong> &t_i : t)
    {
        t_i.resize(n);
        for (ullong &t_i_j : t_i)
            cin >> t_i_j;
    }
    cin >> e[0] >> e[1] >> x[0] >> x[1];

    cout << fastest_way(a, t, e, x) << '\n';

    return 0;
}

ullong fastest_way(const array<vector<ullong>, TWO> &a,
    const array<vector<ullong>, TWO> &t, const array<ullong, TWO> &e,
    const array<ullong, TWO> &x) noexcept
{
    const size_t n = a.front().size();
    array<vector<ullong>, TWO> f = { vector<ullong>(n), vector<ullong>(n) };
    f[0][0] = e[0] + a[0][0];
    f[1][0] = e[1] + a[1][0];

    for (size_t j = 1; j < n; ++j)
    {
        if (f[0][j - 1] + a[0][j] <= f[1][j - 1] + t[1][j - 1] + a[0][j])
        {
            f[0][j] = f[0][j - 1] + a[0][j];
        }
        else
        {
            f[0][j] = f[1][j - 1] + t[1][j - 1] + a[0][j];
        }
        if (f[1][j - 1] + a[1][j] <= f[0][j - 1] + t[0][j - 1] + a[1][j])
        {
            f[1][j] = f[1][j - 1] + a[1][j];
        }
        else
        {
            f[1][j] = f[0][j - 1] + t[0][j - 1] + a[1][j];
        }
    }

    ullong f_star;
    if (f[0][n - 1] + x[0] <= f[1][n - 1] + x[1])
    {
        f_star = f[0][n - 1] + x[0];
    }
    else
    {
        f_star = f[1][n - 1] + x[1];
    }

    return f_star;
}
