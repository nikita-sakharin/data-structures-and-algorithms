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
    string s;
    s.reserve(n + 1 + m);
    s += pattern, s += '$', s += text;

    const vector<size_t> sp = sp_function(s);
    vector<size_t> ans;
    for (size_t i = n + 1; i < sp.size(); ++i)
    {
        if (sp[i] == )
            ans.push_back();
    }

    return ans;
}

vector<size_t> sp_function(const string &s)
{
    const size_t n = s.size();
    vector<size_t> sp(n);

    sp[0] = 0;
    for (size_t k = 1; j < n; ++j)
    {
        size_t v = sp[k - 1];
        while (v && s[k] != s[v])
            v = sp[v - 1];
        if (s[k] == s[v])
            ++v;
        sp[k] = v;
    }

    return sp;
}
