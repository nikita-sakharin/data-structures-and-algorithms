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

// z_block - O(n + m)

vector<size_t> z_block(const string &, const string &);
vector<size_t> z_function(const string &);

int main()
{
    string text, pattern;
    cin >> pattern >> text;
    const vector<size_t> ans = z_block(pattern, text);
    for (const size_t &ans_i : ans)
        cout << ans_i << ' ';
    cout << '\n';

    return 0;
}

vector<size_t> z_block(const string &pattern, const string &text)
{
    const size_t n = pattern.size(), m = text.size();
    string s;
    s.reserve(n + 1 + m);
    s += pattern, s += '$', s += text;

    const vector<size_t> z = z_function(s);
    vector<size_t> ans;

    for (size_t i = n + 1; i < z.size(); ++i)
    {
        if (z[i] == n)
            ans.push_back(i - n - 1);
    }

    return ans;
}

vector<size_t> z_function(const string &s)
{
    const size_t n = s.size();
    vector<size_t> z(n);
    z[0] = MAX_SIZE;

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
