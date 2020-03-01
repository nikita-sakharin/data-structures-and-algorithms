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

static constexpr size_t MAX_SIZE = numeric_limits<size_t>::max();

// knuth_morris_pratt - O(n + m)

vector<size_t> knuth_morris_pratt(const string &, const string &);
vector<size_t> failure_function(const string &s);
vector<size_t> z_function(const string &);

int main()
{
    string text, pattern;
    cin >> pattern >> text;
    const vector<size_t> ans = knuth_morris_pratt(text, pattern);
    for (const size_t &ans_i : ans)
        cout << ans_i << ' ';
    cout << '\n';

    return 0;
}

vector<size_t> knuth_morris_pratt(const string &text, const string &pattern)
{
    const size_t m = text.size(), n = pattern.size();
    vector<size_t> f = failure_function(pattern), ans;
    for (size_t c = 0, p = 0; c + (n - p) - 1 < m; p = f[p] - 1)
    {
        while (p < n && pattern[p] == text[c])
        {
            ++c;
            ++p;
        }
        if (p >= n)
            ans.push_back(c - n);
        if (!p)
            ++c;
    }

    return ans;
}

vector<size_t> failure_function(const string &s)
{
    const size_t n = s.size();
    vector<size_t> z = z_function(s), sp(n, 0);

    for (size_t j = n - 1; j; --j)
    {
        const size_t i = j + z[j] - 1;
        sp[i] = z[j];
    }

    vector<size_t> f(n + 1);
    f[0] = 1;
    for (size_t i = 1; i <= n; ++i)
    {
        f[i] = sp[i - 1] + 1;
    }

    return f;
}

vector<size_t> z_function(const string &s)
{
    const size_t n = s.size();
    vector<size_t> z(n);
    z[0] = 0;

    for (size_t k = 1, l = 0, r = 0; k < n; ++k)
    {
        if (k <= r)
            z[k] = min(r - k + 1, z[k - l]);

        while (k + z[k] < n && s[z[k]] == s[k + z[k]])
            ++z[k];

        if (k + z[k] - 1 > r)
        {
            l = k;
            r = k + z[k] - 1;
        }
    }

    return z;
}
