#include <cassert>
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

// knapsack - O(n^2)

vector<vector<ullong>> knapsack(const vector<ullong> &, const vector<ullong> &,
    ullong) noexcept;

int main()
{
    size_t n;
    ullong w;
    cin >> n >> w;
    vector<ullong> weight(n), price(n);

    for (ullong &weight_i : weight)
        cin >> weight_i;
    for (ullong &price_i : price)
        cin >> price_i;

    const vector<vector<ullong>> a = knapsack(weight, price, w);
    cout << a[n][w] << '\n';

    return 0;
}

vector<vector<ullong>> knapsack(const vector<ullong> &weight,
    const vector<ullong> &price, const ullong w) noexcept
{
    size_t n = weight.size();
    vector<vector<ullong>> a(n + 1, vector<ullong>(w + 1, 0));

    for (size_t k = 1; k <= n; ++k)
    {
        for (size_t s = 1; s <= w; ++s)
        {
            if (s >= weight[k - 1])
            {
                a[k][s] = max(a[k - 1][s],
                    a[k - 1][s - weight[k - 1]] + price[k - 1]);
            }
            else
            {
                a[k][s] = a[k - 1][s];
            }
        }
    }

    return a;
}
