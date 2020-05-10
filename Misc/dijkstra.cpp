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

static constexpr dbl INFINITY_DBL = numeric_limits<dbl>::infinity();

pair<vector<dbl>, vector<llong>> dijkstra(
    const vector<vector<pair<ullong, dbl>>> &, ullong);
void relax(const ullong &, const pair<ullong, dbl> &,
    set<pair<dbl, ullong>> &, vector<dbl> &, vector<llong> &) noexcept;

int main()
{
    ullong n, m, s;
    cin >> n >> m >> s;
    vector<vector<pair<ullong, dbl>>> g(n);

    for (ullong i = 0; i < m; ++i)
    {
        ullong u, v;
        dbl w;
        cin >> u >> v >> w;
        g[u].emplace_back(v, w);
    }

    vector<dbl> d;
    vector<llong> pi;
    tie(d, pi) = dijkstra(g, s);
}

pair<vector<dbl>, vector<llong>> dijkstra(
    const vector<vector<pair<ullong, dbl>>> &g, const ullong s)
{
    const ullong n = g.size();
    vector<dbl> d(n, INFINITY_DBL);
    vector<llong> pi(n, -1);
    d[s] = 0.0;

    set<pair<dbl, ullong>> q;
    q.emplace(d[s], s);

    while (!q.empty())
    {
        const ullong u = q.begin()->second;
        q.erase(q.begin());

        for (const pair<ullong, dbl> &u_v : g[u])
        {
            relax(u, u_v, q, d, pi);
        }
    }

    return make_pair(d, pi);
}

inline void relax(const ullong &u, const pair<ullong, dbl> &u_v,
    set<pair<dbl, ullong>> &q, vector<dbl> &d, vector<llong> &pi) noexcept
{
    const llong v = u_v.first;
    const dbl w = d[u] + u_v.second;

    if (d[v] > w)
    {
        if (!isinf(d[v]))
        {
            q.erase(make_pair(d[v], v));
        }

        d[v] = w;
        pi[v] = u;

        q.emplace(d[v], v);
    }
}
