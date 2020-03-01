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

// levenshtein_distance - O(n^2)

ullong levenshtein_distance(const string &s1, const string &s2,
    const uint delete_cost = 1, const uint insert_cost = 1,
    const uint replace_cost = 1) noexcept;

int main()
{
    string s1, s2;
    cin >> s1 >> s2;
    cout << levenshtein_distance(s1, s2) << '\n';

    return 0;
}

ullong levenshtein_distance(const string &s1, const string &s2,
    const uint delete_cost, const uint insert_cost,
    const uint replace_cost) noexcept
{
    const size_t m = s1.size(), n = s2.size();
    vector<vector<ullong>> d(m + 1, vector<ullong>(n + 1, 0));

    d[0][0] = 0;
    for (size_t j = 1; j <= n; ++j)
    {
        d[0][j] = d[0][j - 1] + insert_cost;
    }

    for (size_t i = 1; i <= m; ++i)
    {
        d[i][0] = d[i - 1][0] + delete_cost;
        for (size_t j = 1; j <= n; ++j)
        {
            if (s1[i - 1] == s2[j - 1])
                d[i][j] = d[i - 1][j - 1];
            else
                d[i][j] = min(d[i - 1][j - 1] + replace_cost, min(
                    d[i][j - 1] + insert_cost,
                    d[i - 1][j] + delete_cost));
        }
    }

    return d[m][n];
}
