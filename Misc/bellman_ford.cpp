#include <cmath>
#include <cstddef>
#include <cctype>

#include <algorithm>
#include <array>
#include <bitset>
#include <deque>
#include <forward_list>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

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

using namespace std;

template<class T>
using iter = typename T::iterator;
template<class T>
using citer = typename T::const_iterator;
template<class T>
using riter = typename T::reverse_iterator;
template<class T>
using criter = typename T::const_reverse_iterator;

static constexpr llong MAX_LLONG = numeric_limits<llong>::max();

tuple<vector<llong>, vector<llong>, bool> bellman_ford(
    const vector<vector<pair<ullong, llong>>> &, ullong);
bool relax(const ullong &, const pair<ullong, llong> &,
    vector<llong> &, vector<llong> &) noexcept;

int main()
{
    ullong n, m, s;
    cin >> n >> m >> s;
    vector<vector<pair<ullong, llong>>> g(n);

    for (ullong i = 0; i < m; ++i)
    {
        ullong u, v;
        llong w;
        cin >> u >> v >> w;
        g[u].emplace_back(v, w);
    }

    vector<llong> d, pi;
    bool is_changed;

    tie(d, pi, is_changed) = bellman_ford(g, s);
    cout << is_changed << '\n';
}

tuple<vector<llong>, vector<llong>, bool> bellman_ford(
    const vector<vector<pair<ullong, llong>>> &g, const ullong s)
{
    const ullong n = g.size();
    vector<llong> d(n, MAX_LLONG), pi(n, -1);
    d[s] = 0;

    bool is_changed = true;
    for (ullong i = 0; i < n && is_changed; ++i)
    {
        is_changed = false;
        for (ullong u = 0; u < n; ++u)
        {
            for (const pair<ullong, llong> &u_v : g[u])
            {
                is_changed |= relax(u, u_v, d, pi);
            }
        }
    }

    return tuple(d, pi, is_changed);
}

inline bool relax(const ullong &u, const pair<ullong, llong> &u_v,
    vector<llong> &d, vector<llong> &pi) noexcept
{
    const llong v = u_v.first, w = d[u] + u_v.second;
    if (d[u] != MAX_LLONG && d[v] > w)
    {
        d[v] = w;
        pi[v] = u;
        return true;
    }

    return false;
}
